import sys
import os

import VSProjectUtil

class ReflectionMember(object):
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

class ReflectionClass(object):
    def __init__(self, className: str, headerPath: str):
        super().__init__()
        self.className = className
        self.parentClasses = []
        self.headerPath = headerPath
        self.members = []

def GenerateReflectionHeaders(solutionFolder: str, projectName: str):
    print("Search for existing Reflection files...")

    existingClasses = []
    intermediatesPath = os.path.join(solutionFolder, "intermediates/generated")
    for filename in os.listdir(intermediatesPath):
        if filename.endswith(".h"):
            parsedClass = ParseReflectionHeader(os.path.join(intermediatesPath, filename))
            if not parsedClass == None:
                existingClasses.append(parsedClass)

    print("Found " + str(len(existingClasses)) + " Reflection files.")

    projectFolder = os.path.join(solutionFolder, projectName)
    print("Generate Reflection files for Folder: " + projectFolder)
    os.chdir(projectFolder)
    reflectedClasses = []
    for root, dirs, files in os.walk("."):
        for file in files:
            filePath = os.path.join(root, file)
            if file.endswith(".h"):
                classMeta = ParseHeaderFile(filePath)
                if not classMeta == None:
                    reflectedClasses.append(classMeta)
    for classMeta in reflectedClasses:
        if ReflectionHeaderNeedsUpdate(classMeta, existingClasses):
            print("Write Reflection file for: " + classMeta.className)
            WriteMetaFiles(classMeta, projectFolder, projectName)
    BuildReflectionHeader(projectFolder, projectName, reflectedClasses)

def ParseReflectionHeader(filepath: str):
    parsedClass = None
    with open(os.path.abspath(filepath), "r") as file:
        line = file.readline()
        lineCount = 0
        while line:
            if line.startswith("//#"):
                if lineCount == 0:
                    classLineParts = line[3:-1].split(",")
                    className = classLineParts[0]
                    headerPath = classLineParts[1]
                    parsedClass = ReflectionClass(className, headerPath)
                elif lineCount == 1:
                    parentClassesLineParts = line[3:-1].split(",")
                    for className in parentClassesLineParts:
                        if len(className) > 0:
                            parsedClass.parentClasses.append(className)
                elif lineCount == 2:
                    membersLineParts = line[3:-1].split(",")
                    for member in membersLineParts:
                        memberParts = member.split(" ")
                        if len(memberParts) == 2:
                            parsedClass.members.append(ReflectionMember(memberParts[1], memberParts[0]))
                line = file.readline()
                lineCount += 1
            else:
                return parsedClass 
    return None

def ParseHeaderFile(relativeFilePath: str):
    if relativeFilePath.endswith("RTTI.h"):
        return None
    classMeta = None
    with open(os.path.abspath(relativeFilePath), "r") as file:
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
                        classMeta = ReflectionClass(className, relativeFilePath)
                        if ":" in line:
                            parentIndexStart = line.index(":")
                            parentClasses = line[parentIndexStart + 1:len(line)].strip()
                            parentClasses = parentClasses.replace("public", "").strip()
                            splitParents = list(map(str.strip, parentClasses.split(",")))
                            classMeta.parentClasses = splitParents
                    else:
                        return None
                    
            elif "SPROPERTY" in line and e < lineCount - 1:
                if classMeta == None:
                    print("""Found SPROPERTY macro in non reflected class!
                    Add SOLUS_CLASS() macro to class declaration!""")
                    continue
                nextLine = lines[e + 1]
                nextLineSplit = nextLine.split(" ")
                for i in range(0, len(nextLineSplit)):
                    nextLineSplit[i] = nextLineSplit[i].strip().replace(";", "")
                classMeta.members.append(ReflectionMember(nextLineSplit[0], nextLineSplit[1]))

    return classMeta

def ReflectionHeaderNeedsUpdate(currentClass: ReflectionClass, existingClasses: list):
    if len(existingClasses) == 0:
        return True
    notExisting = True
    existingClass = None
    for classMeta in existingClasses:
        if classMeta.className == currentClass.className:
            notExisting = False
            existingClass = classMeta
            break
    if notExisting:
        return True
    if not existingClass.headerPath == currentClass.headerPath:
        return True
    if not existingClass.parentClasses == currentClass.parentClasses:
        return True
    if not existingClass.members == currentClass.members:
        return True
    return False

def WriteMetaFiles(classMeta: ReflectionClass, folder: str, projectName: str):
    path = os.path.abspath("..\\intermediates\\generated\\")
    reflectionClassName = classMeta.className + "_Reflection"
    with open(path + os.path.sep + classMeta.className + ".generated.h", "w+") as newHeader:
        newHeader.write("//#" + classMeta.className + "," + classMeta.headerPath + "\n")
        parentClassString = ""
        parentClassCount = len(classMeta.parentClasses)
        for i in range(0, parentClassCount):
            parentClassString += classMeta.parentClasses[i]
            if i < parentClassCount - 1:
                parentClassString += ","
        newHeader.write("//#" + parentClassString + "\n")
        membersString = ""
        memberCount = len(classMeta.members)
        for i in range(0, memberCount):
            membersString += classMeta.members[i].name + " " + classMeta.members[i].type
            if i < memberCount - 1:
                membersString += ","
        newHeader.write("//#" + membersString + "\n")
        newHeader.write("#pragma once\n")
        newHeader.write("#include \"Utility/ClassMetaData.h\"\n")
        newHeader.write("#include <unordered_map>\n")
        newHeader.write("#include <string>\n")
        newHeader.write("#include <functional>\n")
        newHeader.write("\nnamespace Solus\n{\n")

        newHeader.write("class " + classMeta.className + ";\n\n")
        newHeader.write("struct SOLUS_API " + reflectionClassName + " : public ClassMetaData\n{\n")
        newHeader.write("\t" + classMeta.className + "_Reflection();\n\n")
        newHeader.write("\tstruct TypeInfo_" + classMeta.className + " : public TypeInfo\n\t{\n")
        newHeader.write("\t\tTypeInfo_" + classMeta.className + "(const char* name, size_t size, std::function<void* (" + classMeta.className + "*)> accessor)\n\t\t: accessor(accessor)\n\t\t{\n")
        newHeader.write("\t\t\tthis->name = name;\n\t\t\tthis->size = size;\n\t\t}\n\n")
        newHeader.write("\t\tstd::function<void* (" + classMeta.className + "*)> accessor = nullptr;\n\n")
        newHeader.write("\t\tvirtual void* GetValuePtr(void* object) override\n\t\t{\n")
        newHeader.write("\t\t\treturn accessor((" + classMeta.className + "*)object);\n\t\t}\n")
        newHeader.write("\n\t};\n\n")
        for member in classMeta.members:
            newHeader.write("\tvoid* " + member.name + "(" + classMeta.className + "* object);\n")

        newHeader.write("};\n")
        newHeader.write("}\n")

    with open(path + os.path.sep + classMeta.className + ".generated.cpp", "w+") as newSource:
        newSource.write("#include \"..\\intermediates\\generated\\" + classMeta.className + ".generated.h\"\n")
        newSource.write("#include \"" + classMeta.headerPath + "\"\n")
        newSource.write("namespace Solus\n{\n")
        newSource.write(reflectionClassName + "::" + reflectionClassName + "()\n{\n")
        newSource.write("\tthis->name = \"" + classMeta.className + "\";\n")
        newSource.write("\tthis->size = sizeof(" + classMeta.className + ");\n")
        for parentClass in classMeta.parentClasses:
            newSource.write("\tthis->parents.push_back(&" + parentClass + "::Reflection); \n")
        newSource.write("\n")
        for member in classMeta.members:
            newSource.write("\tdata[\"" + member.name + "\"] = new TypeInfo_" + classMeta.className + "(\"" + member.type + "\", sizeof(" + member.type + "), std::bind(&" + reflectionClassName + "::" + member.name + ", this, std::placeholders::_1));\n")
            newSource.write("\tsortedMemberKeys.push_back(\"" + member.name + "\");\n")
        newSource.write("\n\tstd::sort(sortedMemberKeys.begin(), sortedMemberKeys.end(), [](std::string a, std::string b){return a < b;});\n")
        newSource.write("}\n\n")
        for member in classMeta.members:
            newSource.write("void* " + reflectionClassName + "::" + member.name + "(" + classMeta.className + "* object)\n{\n")
            newSource.write("\treturn (void*)&object->" + member.name + ";\n")
            newSource.write("}\n\n")
        newSource.write("}\n")

    helper = VSProjectUtil.VcxprojHelper(folder, projectName)
    helper.AddHeader(classMeta.className)
    helper.AddSource(classMeta.className)
    helper.WriteFiles()

def BuildReflectionHeader(folder: str, projectName: str, reflectedClasses: list):
    if len(reflectedClasses) == 0:
        return
    path = os.path.abspath("..\\intermediates\\generated\\")
    with open(path + os.path.sep + projectName + ".generated.h", "w+") as newHeader:
        content = ("#include \"Engine/SolusEngine.h\"\n"
                "#include <typeinfo>\n\n")

        for i in range(0, len(reflectedClasses)):
            headerPath = reflectedClasses[i].headerPath
            content += "#include \"" + headerPath + "\"\n"

        content += ("\nnamespace Solus\n{\n"
        "void " + projectName + "_Init()\n{\n")

        for i in range(0, len(reflectedClasses)):
            meta = reflectedClasses[i]
            content += "\tSolus::ClassMetaData::Insert(typeid(" + meta.className + ").hash_code(), &Solus::" + meta.className + "::Reflection);\n"

        content += "}\n}\n"
        newHeader.write(content)
    
    helper = VSProjectUtil.VcxprojHelper(folder, projectName)
    helper.AddHeader("SolusEngine")
    helper.WriteFiles()

if __name__ == '__main__':
    solutionFolder = sys.argv[1] # "E:\\Projects\\SolusEngine\\"
    projectName = sys.argv[2] # "SolusEngine"
    GenerateReflectionHeaders(solutionFolder, projectName)