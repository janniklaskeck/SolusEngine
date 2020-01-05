import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import os
import uuid
import locale

class VcxprojHelper(object):

    def __init__(self, folder, projectName):
        super().__init__()
        ET.register_namespace("", "http://schemas.microsoft.com/developer/msbuild/2003")
        
        self.vcxprojXMLFilename = folder + os.path.sep + projectName + ".vcxproj"
        self.vcxprojXML = ET.parse(self.vcxprojXMLFilename)

        self.filterXMLFilename = folder + os.path.sep + projectName + ".vcxproj.filters"
        self.filterXML = ET.parse(self.filterXMLFilename)
        
        self.vcxprojRoot = self.vcxprojXML.getroot()
        self.filterRoot = self.filterXML.getroot()
        
        self.hasChanges = False

        self.__EnsureGeneratedFilter()
        
    def Print(self):
        print(ET.tostring(self.vcxprojRoot, encoding="unicode"))

    def AddHeader(self, filename):
        self.__AddFilterHeader(filename + ".generated.h")
        self.__AddProjectHeader(filename + ".generated.h")

    def AddSource(self, filename):
        self.__AddFilterSource(filename + ".generated.cpp")
        self.__AddProjectSource(filename + ".generated.cpp")

    def WriteFiles(self):
        if self.hasChanges:
            self.__WriteFilter()
            self.__WriteVcxproj()

#############################

    def __WriteElementTree(self, rootElement, filename):
        string = ET.tostring(rootElement, encoding="unicode")
        with open(filename, "w") as file:
            file.write('<?xml version="1.0" encoding="utf-8"?>\n')
            file.write(string)

    def __WriteFilter(self):
        self.__WriteElementTree(self.filterRoot, self.filterXMLFilename)

    def __WriteVcxproj(self):
        self.__WriteElementTree(self.vcxprojRoot, self.vcxprojXMLFilename)

    def __AddFilterHeader(self, filename):
        headerItemGroup = self.__GetFilterHeaderElement()
        includePath = "..\\intermediates\\generated\\" + filename
        for child in headerItemGroup:
            if "Include" in child.attrib and child.attrib["Include"] == includePath:
                return
        self.hasChanges = True
        newChild = ET.Element("ClInclude")
        newChild.set("Include", includePath)
        filterChild = ET.Element("Filter")
        filterChild.text = "Generated"
        newChild.append(filterChild)
        headerItemGroup.append(newChild)

    def __AddFilterSource(self, filename):
        sourceItemGroup = self.__GetFilterSourceElement()
        includePath = "..\\intermediates\\generated\\" + filename
        for child in sourceItemGroup:
            if "Include" in child.attrib and child.attrib["Include"] == includePath:
                return
        self.hasChanges = True
        newChild = ET.Element("ClCompile")
        newChild.set("Include", includePath)
        filterChild = ET.Element("Filter")
        filterChild.text = "Generated"
        newChild.append(filterChild)
        sourceItemGroup.append(newChild)

    def __EnsureGeneratedFilter(self):
        filterItemGroup = self.__GetFilterFilterElement()
        foundFilter = False
        for filterElement in filterItemGroup:
            if "Include" in filterElement.attrib and filterElement.attrib["Include"] == "Generated":
                foundFilter = True
        if foundFilter:
            return
        self.hasChanges = True
        generatedFilter = ET.Element("Filter")
        generatedFilter.set("Include", "Generated")
        generatedUUID = ET.Element("UniqueIdentifier")
        generatedUUID.text = "{" + str(uuid.uuid4()) + "}"
        generatedFilter.append(generatedUUID)
        filterItemGroup.append(generatedFilter)

    def __GetFilterFilterElement(self):
        for itemgroup in self.filterRoot:
            if itemgroup[0].tag.endswith("Filter"):
                return itemgroup
        return None

    def __GetFilterSourceElement(self):
        for itemgroup in self.filterRoot:
            if itemgroup[0].tag.endswith("ClCompile"):
                return itemgroup
        return None

    def __GetFilterHeaderElement(self):
        for itemgroup in self.filterRoot:
            if itemgroup[0].tag.endswith("ClInclude"):
                return itemgroup
        return None

######################################

    def __AddProjectHeader(self, filename):
        headerItemGroup = self.__GetProjectHeaderElement()
        includePath = "..\\intermediates\\generated\\" + filename
        for child in headerItemGroup:
            if "Include" in child.attrib and child.attrib["Include"] == includePath:
                return
        self.hasChanges = True
        newChild = ET.Element("ClInclude")
        newChild.set("Include", includePath)
        headerItemGroup.append(newChild)

    def __AddProjectSource(self, filename):
        sourceItemGroup = self.__GetProjectSourceElement()
        includePath = "..\\intermediates\\generated\\" + filename
        for child in sourceItemGroup:
            if "Include" in child.attrib and child.attrib["Include"] == includePath:
                return
        self.hasChanges = True
        newChild = ET.Element("ClCompile")
        newChild.set("Include", includePath)
        sourceItemGroup.append(newChild)

    def __GetProjectHeaderElement(self):
        for child in self.vcxprojRoot:
            if len(child) > 0 and child.tag.endswith("ItemGroup"):
                if child[0].tag.endswith("ClInclude"):
                    return child
        return None

    def __GetProjectSourceElement(self):
        for child in self.vcxprojRoot:
            if len(child) > 0 and child.tag.endswith("ItemGroup"):
                if child[0].tag.endswith("ClCompile"):
                    return child
        return None

if __name__ == '__main__':
    test = VcxprojHelper("../SolusEngine", "SolusEngine")
    test.AddHeader("FreeFlyCamera")
    test.AddSource("FreeFlyCamera")
    test.Print()
    #test.WriteFiles()