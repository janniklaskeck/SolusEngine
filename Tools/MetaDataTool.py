import os

import VSProjectUtil

class MetaDataMember(object):
    def __init__(self, memberType: str, memberName: str):
        super().__init__()
        self.type = memberType
        self.name = memberName

    def __repr__(self):
        return "({} {})".format(self.name, self.type)

    def __hash__(self):
        return hash((self.name, self.type))

    def __eq__(self, value):
        try:
            return (self.name, self.type) == (value.name, value.type)
        except AttributeError:
            return NotImplemented

class MetaDataClass(object):
    def __init__(self, filePath: str):
        super().__init__()
        self.filePath = filePath
        self.className = ""
        self.parentClasses = []
        self.headerPath = ""
        self.members = []

    def ParseMetaDataClass(self):
        # Only parse beginning comments of header file, all information is there
        # Each comment starts with '#'
        # First line is classname and relative header path
        # Second line are parent classes, comma seperated
        # Third line are member variables name and type, comma seperated
        with open(os.path.abspath(self.filePath), "r") as file:
            line = file.readline()
            lineCount = 0
            while line:
                if line.startswith("//#"):
                    if lineCount == 0:
                        classLineParts = line[3:-1].split(",")
                        self.className = classLineParts[0]
                        self.headerPath = classLineParts[1]
                    elif lineCount == 1:
                        parentClassesLineParts = line[3:-1].split(",")
                        for className in parentClassesLineParts:
                            if len(className) > 0:
                                self.parentClasses.append(className)
                    elif lineCount == 2:
                        membersLineParts = line[3:-1].split(",")
                        for member in membersLineParts:
                            memberParts = member.split(" ")
                            if len(memberParts) == 2:
                                self.members.append(MetaDataMember(memberParts[1], memberParts[0]))
                    line = file.readline()
                    lineCount += 1
                else:
                    return True
        return False

    def ParseHeaderClass(self):
        # Special case
        if self.filePath.endswith("RTTI.h"):
            return False
        with open(os.path.abspath(self.filePath), "r") as file:
            lines = file.readlines()
            lineCount = len(lines)
            for e in range(0, lineCount):
                line = lines[e]
                lineParts = line.split(" ")
                for i in range(0, len(lineParts)):
                    lineParts[i] = lineParts[i].strip()
                if ";" in lineParts:
                    continue
                if "class" in lineParts and "SOLUS_CLASS" in lines[e-1]:
                    if lineParts[len(lineParts) - 1].endswith(";"):
                        continue
                    classIndex = lineParts.index("class")
                    if classIndex >= 0:
                        className = lineParts[classIndex + 1]
                        if className.endswith(";"):
                            continue
                        if className == "SOLUS_API":
                            className = lineParts[classIndex + 2]
                        isReflected = False
                        if e > 0:
                            self.className = className
                            self.headerPath = self.filePath
                            if ":" in line:
                                parentIndexStart = line.index(":")
                                parentClasses = line[parentIndexStart + 1:len(line)].strip()
                                parentClasses = parentClasses.replace("public", "").strip()
                                splitParents = list(map(str.strip, parentClasses.split(",")))
                                self.parentClasses = splitParents
                                self.parentClasses.sort()
                        else:
                            return False
                        
                elif "SPROPERTY" in line and e < lineCount - 1:
                    if len(self.className) == 0:
                        print("""Found SPROPERTY macro in non marked class '""" + self.filePath + 
                        """'\nAdd SOLUS_CLASS() macro to class declaration!""")
                        continue
                    nextLine = lines[e + 1]
                    declaration = nextLine.replace(";", "").strip()
                    declarationDefaultValueStart = declaration.rfind("=")
                    if declarationDefaultValueStart == -1:
                        nameStartIndex = declaration.rfind(" ")
                        memberName = declaration[nameStartIndex+1:].strip()    
                    else:
                        declaration = declaration[:declarationDefaultValueStart-1]
                        nameStartIndex = declaration.rfind(" ")
                        memberName = declaration[nameStartIndex+1:].strip()    
                    
                    memberType = declaration[:nameStartIndex].strip()
                    #typeNames.append(TypeNameHeader(memberType, relativeFilePath))
                    self.members.append(MetaDataMember(memberType, memberName))
        return len(self.className) > 0

    def IsOutdated(self, metaDataClassList: list):
        existingClass = None
        for metaClass in metaDataClassList:
            if metaClass.className == self.className:
                existingClass = metaClass
                break
        
        if existingClass == None:
            return True
        
        if not existingClass.parentClasses == self.parentClasses:
            return True

        if not existingClass.headerPath == self.headerPath:
            return True

        if not len(existingClass.members) == len(self.members):
            return True

        memberCopy = list(self.members)

        for i in range(0, len(self.members)):
            currentMember = memberCopy.pop()
            for member in existingClass.members:
                if not member.name == currentMember.name and not member.type == currentMember.type:
                    # Put back to list to show not matching member
                    memberCopy.append(currentMember)

        if len(memberCopy) > 0:
            return True
        
        return False

def GenerateMetaDataClasses(rootFolder: str, subFolder: str):
    existingClasses = GetExistingMetaDataClasses(rootFolder)

    projectFolder = os.path.join(rootFolder, subFolder)
    print("Generate meta data files for folder: " + projectFolder)
    os.chdir(projectFolder)
    reflectedClasses = []
    for root, dirs, files in os.walk("."):
        for file in files:
            filePath = os.path.join(root, file)
            if file.endswith(".h"):
                classMeta = MetaDataClass(filePath)
                if classMeta.ParseHeaderClass():
                    reflectedClasses.append(classMeta)
    for classMeta in reflectedClasses:
        #if classMeta.IsOutdated(existingClasses):
            #print("Write meta data class for: " + classMeta.className)
        WriteMetaFiles(classMeta, projectFolder, subFolder)
    #BuildReflectionHeader(projectFolder, subFolder, reflectedClasses)

def WriteMetaFiles(classMeta: MetaDataClass, folder: str, projectName: str):
    WriteMetaHeader(classMeta, folder, projectName)
    WriteMetaSource(classMeta, folder, projectName)

    helper = VSProjectUtil.VcxprojHelper(folder, projectName)
    helper.AddHeader(classMeta.className)
    helper.AddSource(classMeta.className)
    helper.WriteFiles()

def WriteMetaHeader(classMeta: MetaDataClass, folder: str, projectName: str):
    path = os.path.abspath("..\\intermediates\\generated\\")
    reflectionClassName = classMeta.className + "_ClassMetaData"
    
    headerString = "//#" + classMeta.className + "," + classMeta.headerPath + "\n"
    parentClassString = ""
    parentClassCount = len(classMeta.parentClasses)
    for i in range(0, parentClassCount):
        parentClassString += classMeta.parentClasses[i]
        if i < parentClassCount - 1:
            parentClassString += ","
    headerString += "//#" + parentClassString + "\n"
    membersString = ""
    memberCount = len(classMeta.members)
    for i in range(0, memberCount):
        membersString += classMeta.members[i].name + " " + classMeta.members[i].type
        if i < memberCount - 1:
            membersString += ","
    headerString += "//#" + membersString + "\n"
    headerString += "#pragma once\n"
    for parent in classMeta.parentClasses:
        headerString += "#include \"" + parent + ".generated.h\"\n"
    headerString += "#include \"Utility/ClassMetaData.h\"\n"
    headerString += "#include <string>\n"
    headerString += "\nnamespace Solus\n{\n"

    #headerString += "class " + classMeta.className + ";\n\n"
    parentClass = "ClassMetaData"
    if len(classMeta.parentClasses) > 0:
        parentClass = classMeta.parentClasses[0] + "_ClassMetaData"
    headerString += "struct SOLUS_API " + reflectionClassName + " : public " + parentClass + "\n{\n"
    headerString += "\t" + classMeta.className + "_ClassMetaData();\n\n"

    headerString += "\tvirtual void Serialize(ArchiveStream* archive, const SolusObject* object) const override;\n"
    headerString += "\tvirtual void Deserialize(ArchiveStream* archive, SolusObject* object) override;\n"
    headerString += "\tvirtual const bool DeserializeMember(const SolusObject* object, const std::string& name, const unsigned char* ptr, uint32_t length) override;\n"
    headerString += "\tvirtual void* GetMemberPtrInternal(const SolusObject* object, const std::string& name) override;\n"
    headerString += "};\n"
    headerString += "}\n"

    headerPath = path + os.path.sep + classMeta.className + ".generated.h"
    oldHeaderContentSize = 0
    oldHeaderString = ""
    if os.path.exists(headerPath):
        with open(headerPath, "r") as oldHeader:
            oldHeaderString = oldHeader.read()
            oldHeaderContentSize = len(oldHeaderString)

    if not oldHeaderContentSize == len(headerString) or not oldHeaderString == headerString:
        print("Write meta data header for: " + classMeta.className)
        with open(headerPath, "w+") as newHeader:
            newHeader.write(headerString)

def WriteMetaSource(classMeta: MetaDataClass, folder: str, projectName: str):
    path = os.path.abspath("..\\intermediates\\generated\\")
    reflectionClassName = classMeta.className + "_ClassMetaData"

    sourceString = "#include \"..\\intermediates\\generated\\" + classMeta.className + ".generated.h\"\n"
    sourceString += "#include \"" + classMeta.headerPath + "\"\n"
    sourceString += "#include \"Utility/SerializeUtil.h\"\n\n"
    
    sourceString += "RTTR_REGISTRATION\n{\n"
    sourceString += "\trttr::registration::class_<Solus::" + classMeta.className + ">(\"" + classMeta.className + "\")\n"
    #sourceString += "\t.constructor([](){return new Solus::" + classMeta.className + ";})\n"
    sourceString += "\t.constructor()\n"
    sourceString += "\t(rttr::policy::ctor::as_raw_pointer_get())\n"
    sourceString += "\t.property_readonly(\"MetaData\", &Solus::" + classMeta.className + "::MetaData)\n"
    for member in classMeta.members:
        sourceString += "\t.property(\"" + member.name + "\", &Solus::" + classMeta.className + "::" + member.name + ")\n"
        if member.type.startswith("std::vector"):
            sourceString += "\t(rttr::policy::prop::as_reference_wrapper_get())\n"
    sourceString += "\t;\n}\n\n"


    sourceString += "namespace Solus\n{\n"
    sourceString += reflectionClassName + "::" + reflectionClassName + "()\n{\n"
    sourceString += "}\n\n"


    sourceString += "void " + reflectionClassName + "::Serialize(ArchiveStream* archive, const SolusObject* object) const\n{\n"
    sourceString += "\t" + classMeta.className + "* cast = (" + classMeta.className + "*)object;\n"
    sourceString += "\tstd::string typeName;\n"
    for member in classMeta.members:
        #sourceString += "\ttypeName = std::string(cast->get_type().get_property(\"" + member.name + "\").get_type().get_name());\n"
        sourceString += "\ttypeName = \"" + member.type + "\";\n"
        sourceString += "\tarchive->Serialize(\"" + member.name + "\", typeName, cast->" + member.name + ");\n"
    if len(classMeta.parentClasses) > 0:
        sourceString += "\t" + classMeta.parentClasses[0] + "_ClassMetaData::Serialize(archive, object);\n"
    sourceString += "}\n\n"


    sourceString += "void " + reflectionClassName + "::Deserialize(ArchiveStream* archive, SolusObject* object)\n{\n"
    sourceString += "\t" + classMeta.className + "* cast = (" + classMeta.className + "*)object;\n"
    sourceString += "\tstd::string typeName;\n"
    for member in classMeta.members:
        sourceString += "\ttypeName = \"" + member.type + "\";\n"
        sourceString += "\tarchive->Deserialize(object, typeName, \"" + member.name + "\", cast->" + member.name + ");\n"
    if len(classMeta.parentClasses) > 0:
        sourceString += "\t" + classMeta.parentClasses[0] + "_ClassMetaData::Deserialize(archive, object);\n"
    sourceString += "}\n\n"


    sourceString += "\nconst bool " + reflectionClassName + "::DeserializeMember(const SolusObject* object, const std::string& name, const unsigned char* ptr, uint32_t length)\n{\n"
    sourceString += "\t" + classMeta.className + "* cast = (" + classMeta.className +  "*)object;\n"
    for member in classMeta.members:
        sourceString += "\tif (name == \"" + member.name + "\")\n\t{\n"
        sourceString += "\t\tvoid* dst = &cast->" + member.name + ";\n"
        sourceString += "\t\tmemcpy(dst, ptr, length);\n\t\treturn true;\n\t}\n"
    if len(classMeta.parentClasses) > 0:
        sourceString += "\treturn " + classMeta.parentClasses[0] + "_ClassMetaData::DeserializeMember(object, name, ptr, length);\n"
    else:
        sourceString += "\treturn false;\n"
    sourceString += "}\n\n"


    sourceString += "\nvoid* " + reflectionClassName + "::GetMemberPtrInternal(const SolusObject* object, const std::string& name)\n{\n"
    sourceString += "\t" + classMeta.className + "* cast = (" + classMeta.className +  "*)object;\n"
    for member in classMeta.members:
        sourceString += "\tif (name == \"" + member.name + "\")\n\t{\n"
        sourceString += "\t\treturn &cast->" + member.name + ";\n\t}\n"
    if len(classMeta.parentClasses) > 0:
        sourceString += "\treturn " + classMeta.parentClasses[0] + "_ClassMetaData::GetMemberPtrInternal(object, name);\n"
    else:
        sourceString += "\treturn nullptr;\n"
    sourceString += "}\n\n"


    sourceString += "}\n"

    sourcePath = path + os.path.sep + classMeta.className + ".generated.cpp"
    oldSourceContentSize = 0
    oldSourceString = ""
    if os.path.exists(sourcePath):
        with open(sourcePath, "r") as oldSource:
            oldSourceString = oldSource.read()
            oldSourceContentSize = len(oldSourceString)

    if not oldSourceContentSize == len(sourceString) or not oldSourceString == sourceString:
        print("Write meta data source for: " + classMeta.className)
        with open(sourcePath, "w+") as newSource:
            newSource.write(sourceString)

def GetExistingMetaDataClasses(folder: str):
    print("Search for existing meta data headers...")

    existingClasses = []
    intermediatesPath = os.path.join(folder, "intermediates/generated")
    if not os.path.exists(intermediatesPath):
        os.makedirs(intermediatesPath)
    for filename in os.listdir(intermediatesPath):
        if filename.endswith(".h"):
            parsedClass = MetaDataClass(os.path.join(intermediatesPath, filename))
            if parsedClass.ParseMetaDataClass():
                existingClasses.append(parsedClass)

    print("Found " + str(len(existingClasses)) + " meta data headers in " + intermediatesPath)
    return existingClasses


if __name__ == '__main__':
    solutionFolder = "E:\\Projects\\SolusEngine\\"
    projectName = "SolusEngine"
    GenerateMetaDataClasses(solutionFolder, projectName)
