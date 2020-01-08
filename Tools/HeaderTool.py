import sys
import os

import VSProjectUtil

class ReflectionMember(object):
    def __init__(self, memberType, memberName):
        super().__init__()
        self.type = memberType
        self.name = memberName

class ReflectionClass(object):
    def __init__(self, className, headerPath):
        super().__init__()
        self.className = className
        self.headerPath = headerPath
        self.members = []

def GenerateReflectionHeaders(folder, projectName):
    print("Generate Reflection files for Folder: " + folder + " ProjectName: " + projectName)
    os.chdir(folder)
    reflectedClasses = []
    for root, dirs, files in os.walk("."):
        for file in files:
            filePath = os.path.join(root, file)
            if file.endswith(".h"):
                classMeta = ParseHeaderFile(filePath)
                if not classMeta == None:
                    reflectedClasses.append(classMeta)
    for classMeta in reflectedClasses:
        UpdateMetaFiles(classMeta, folder, projectName)
    BuildReflectionHeader(folder, projectName, reflectedClasses)

def ParseHeaderFile(relativeFilePath):
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
            if "class" in lineParts:
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
                    if e > 0 and "SOLUS_CLASS" in lines[e-1]:
                        classMeta = ReflectionClass(className, relativeFilePath)
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

def UpdateMetaFiles(classMeta, folder, projectName):
    path = os.path.abspath("..\\intermediates\\generated\\")
    reflectionClassName = classMeta.className + "_Reflection"
    with open(path + os.path.sep + classMeta.className + ".generated.h", "w+") as newHeader:
        newHeader.write("#pragma once\n")
        newHeader.write("#include \"Utility/RTTI.h\"\n")
        newHeader.write("#include <unordered_map>\n")
        newHeader.write("#include <string>\n")
        newHeader.write("namespace Solus\n{\n")

        newHeader.write("class " + classMeta.className + ";\n")
        newHeader.write("struct SOLUS_API " + reflectionClassName + " : public ClassMetaData\n{\n")
        newHeader.write(classMeta.className + "_Reflection();\n")
        newHeader.write("struct TypeInfo_" + classMeta.className + " : public TypeInfo\n{\n")
        newHeader.write("TypeInfo_" + classMeta.className + "(const char* name, size_t size, std::function<void* (" + classMeta.className + "*)> accessor)\n: accessor(accessor)\n{\n")
        newHeader.write("this->name = name;\nthis->size = size;\n}\n")
        newHeader.write("std::function<void* (" + classMeta.className + "*)> accessor = nullptr;\n")
        newHeader.write("virtual void* GetValuePtr(void* object) override\n{\n")
        newHeader.write("return accessor((" + classMeta.className + "*)object);\n}\n")
        newHeader.write("};\n\n")
        for member in classMeta.members:
            newHeader.write("void* " + member.name + "(" + classMeta.className + "* object);\n")

        newHeader.write("};\n")
        newHeader.write("}\n")

    with open(path + os.path.sep + classMeta.className + ".generated.cpp", "w+") as newSource:
        newSource.write("#include \"..\\intermediates\\generated\\" + classMeta.className + ".generated.h\"\n")
        newSource.write("#include \"" + classMeta.headerPath + "\"\n")
        newSource.write("namespace Solus\n{\n")
        newSource.write(reflectionClassName + "::" + reflectionClassName + "()\n{\n")
        for member in classMeta.members:
            newSource.write("data[\"" + member.name + "\"] = new TypeInfo_" + classMeta.className + "(\"" + member.type + "\", sizeof(" + member.type + "), std::bind(&" + reflectionClassName + "::" + member.name + ", this, std::placeholders::_1));\n")
        newSource.write("this->name = \"" + classMeta.className + "\";\n")
        newSource.write("this->size = sizeof(" + classMeta.className + ");\n")
        newSource.write("}\n\n")
        for member in classMeta.members:
            newSource.write("void* " + reflectionClassName + "::" + member.name + "(" + classMeta.className + "* object)\n{\n")
            newSource.write("return (void*)&object->" + member.name + ";\n")
            newSource.write("}\n")
        newSource.write("}\n")

    helper = VSProjectUtil.VcxprojHelper(folder, projectName)
    helper.AddHeader(classMeta.className)
    helper.AddSource(classMeta.className)
    helper.WriteFiles()

def BuildReflectionHeader(folder, projectName, reflectedClasses):
    if len(reflectedClasses) == 0:
        return
    path = os.path.abspath("..\\intermediates\\generated\\")
    with open(path + os.path.sep + projectName + ".generated.h", "w+") as newHeader:
        content = ("#include \"Engine/SolusEngine.h\"\n"
                "#include <typeinfo>\n\n")

        for i in range(0, len(reflectedClasses)):
            headerPath = reflectedClasses[i].headerPath
            content += "#include \"" + headerPath + "\"\n"

        content += ("\nnamespace Solus {\n"
        "void " + projectName + "_Init()\n{\n")

        for i in range(0, len(reflectedClasses)):
            meta = reflectedClasses[i]
            content += "Solus::ClassMetaData::Insert(typeid(" + meta.className + ").hash_code(), &Solus::" + meta.className + "::Reflection);\n"

        content += "}\n}\n"
        newHeader.write(content)
    
    helper = VSProjectUtil.VcxprojHelper(folder, projectName)
    helper.AddHeader("SolusEngine")
    helper.WriteFiles()

if __name__ == '__main__':
    GenerateReflectionHeaders(sys.argv[1], sys.argv[2])