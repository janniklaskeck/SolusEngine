import sys
import os

import VSProjectUtil

class ReflectionMember(object):
    def __init__(self, memberName):
        super().__init__()
        self.name = memberName

class ReflectionClass(object):
    def __init__(self, className, headerPath):
        super().__init__()
        self.className = className
        self.headerPath = headerPath
        self.members = []

def GenerateReflectionHeaders(folder):
    os.chdir(folder)
    reflectedClasses = []
    for root, dirs, files in os.walk("."):
        for file in files:
            filePath = os.path.join(root, file)
            if file.endswith(".h"):
                classMeta = ParseHeaderFile(filePath)
                if not classMeta == None:
                    reflectedClasses.append(classMeta)

                #isReflected, className = ContainsReflectedClass(filePath)
                #if isReflected:
                #    reflectedClasses.append(ReflectionClass(className, os.path.join(root, file)))
    for classMeta in reflectedClasses:
        UpdateMetaFiles(classMeta)
    #BuildReflectionHeader(folder, reflectedClasses)

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
                classMeta.members.append(ReflectionMember(nextLineSplit[1]))

    return classMeta

def ContainsReflectedClass(filename):
    with open(filename, "r") as file:
        lines = file.readlines()
        for e in range(0, len(lines)):
            line = lines[e]
            lineParts = line.split(" ")
            for i in range(0, len(lineParts)):
                lineParts[i] = lineParts[i].strip()
            if ";" in lineParts:
                continue
            if "class" in lineParts:
                classIndex = lineParts.index("class")
                if classIndex >= 0:
                    className = lineParts[classIndex + 1]
                    if className.endswith(";"):
                        continue
                    if className == "SOLUS_API":
                        className = lineParts[classIndex + 2]
                    isReflected = False
                    if e > 0:
                        isReflected = "SOLUS_CLASS" in lines[e-1]
                    return isReflected, className
    return False, "ERROR"

def UpdateMetaFiles(classMeta):
    path = os.path.abspath("..\\intermediates\\generated\\")
    reflectionClassName = classMeta.className + "_Reflection"
    with open(path + os.path.sep + classMeta.className + ".generated.h", "w+") as newHeader:
        newHeader.write("#pragma once\n")
        newHeader.write("#include \"Engine/SolusEngine.h\"\n")
        newHeader.write("#include <unordered_map>\n")
        newHeader.write("#include <string>\n")
        newHeader.write("namespace Solus\n{\n")

        newHeader.write("class " + classMeta.className + ";\n")
        newHeader.write("struct SOLUS_API " + reflectionClassName + "\n{\n")
        newHeader.write(classMeta.className + "_Reflection();\n")
        newHeader.write("std::unordered_map<std::string, void*(" + reflectionClassName + "::*)(" + classMeta.className + "*)> functions;\n")

        for member in classMeta.members:
            newHeader.write("void* " + member.name + "(" + classMeta.className + "* object);\n")

        newHeader.write("void* GetValuePtr(const char* name, " + classMeta.className + "* object);\n")
        newHeader.write("};\n")
        newHeader.write("}\n")

    with open(path + os.path.sep + classMeta.className + ".generated.cpp", "w+") as newSource:
        newSource.write("#include \"..\\intermediates\\generated\\" + classMeta.className + ".generated.h\"\n")
        newSource.write("#include \"" + classMeta.headerPath + "\"\n")
        newSource.write("namespace Solus\n{\n")
        newSource.write(reflectionClassName + "::" + reflectionClassName + "()\n{\n")
        for member in classMeta.members:
            newSource.write("functions[\"" + member.name + "\"] = &" + reflectionClassName + "::" + member.name + ";\n")
        newSource.write("}\n")
        for member in classMeta.members:
            newSource.write("void* " + reflectionClassName + "::" + member.name + "(" + classMeta.className + "* object)\n{\n")
            newSource.write("return (void*)&object->" + member.name + ";\n")
            newSource.write("}\n")
        newSource.write("void* " + reflectionClassName + "::GetValuePtr(const char* name, " + classMeta.className + "* object)\n{\n")
        newSource.write("return std::invoke(functions[name], this, object);\n")
        newSource.write("}\n")
        newSource.write("}\n")

    helper = VSProjectUtil.VcxprojHelper("..\\SolusEngine", "SolusEngine")
    helper.AddHeader(classMeta.className)
    helper.AddSource(classMeta.className)
    helper.WriteFiles()

def BuildReflectionHeader(folder, reflectedClasses):
    if len(reflectedClasses) == 0:
        return
    folderOnly = os.path.basename(os.path.dirname(folder))
    with open(folder + folderOnly + ".generated.h", "w+") as newHeader:
        content = ("#include \"Engine/SolusEngine.h\"\n"
                "#include <typeinfo>\n\n"
                )

        for i in range(0, len(reflectedClasses)):
            headerPath = reflectedClasses[i].headerPath
            content += "#include \"" + headerPath + "\"\n"

        content += ("\nnamespace Solus {\n"
        "void " + folderOnly + "_Init()\n{\n")

        for i in range(0, len(reflectedClasses)):
            meta = reflectedClasses[i]
            content += "Solus::SolusObject::Insert(typeid(" + meta.className + ").hash_code(), &Solus::" + meta.className + "::Reflection);\n"

        content += "}\n}\n"
        newHeader.write(content)

if __name__ == '__main__':
    numArgs = len(sys.argv)
    if numArgs - 1 > 0: # remove file from args
        for i in range(1, numArgs):
            GenerateReflectionHeaders(sys.argv[i])