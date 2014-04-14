##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=QTTest
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/itztli/workspace/Modellsimu/termin1/modellbildung"
ProjectPath            := "/home/itztli/workspace/Modellsimu/termin1/modellbildung"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Fabian Maul  
Date                   :=04/14/2014
CodeLitePath           :="/home/itztli/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g -W $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." "$(IncludeSwitch)/usr/include/qt4" "$(IncludeSwitch)/usr/include/qt4/QtGui" 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)QtCore $(LibrarySwitch)QtGui 
LibPath                := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)/usr/lib/qt4" 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/Schwingung$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/itztli/workspace/Modellsimu/termin1/modellbildung/main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/itztli/workspace/Modellsimu/termin1/modellbildung/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/home/itztli/workspace/Modellsimu/termin1/modellbildung/main.cpp"

$(IntermediateDirectory)/Schwingung$(ObjectSuffix): Schwingung.cpp $(IntermediateDirectory)/Schwingung$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/itztli/workspace/Modellsimu/termin1/modellbildung/Schwingung.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Schwingung$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Schwingung$(DependSuffix): Schwingung.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Schwingung$(ObjectSuffix) -MF$(IntermediateDirectory)/Schwingung$(DependSuffix) -MM "/home/itztli/workspace/Modellsimu/termin1/modellbildung/Schwingung.cpp"

$(IntermediateDirectory)/Schwingung$(PreprocessSuffix): Schwingung.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Schwingung$(PreprocessSuffix) "/home/itztli/workspace/Modellsimu/termin1/modellbildung/Schwingung.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Schwingung$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Schwingung$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Schwingung$(PreprocessSuffix)
	$(RM) $(OutputFile)


