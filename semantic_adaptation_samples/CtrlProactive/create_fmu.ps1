function create-zip([String] $aDirectory, [String] $aZipfile){
    [string]$pathToZipExe = "$($Env:ProgramFiles)\7-Zip\7z.exe";
    [Array]$arguments = "a", "-tzip", "$aZipfile", "$aDirectory", "-r";
    & $pathToZipExe $arguments;
}

function createEmptyDir([String] $aDirectory){
	If(test-path $aDirectory)
	{
		Remove-Item -Recurse -Force $aDirectory
	}
	New-Item -ItemType Directory -Path $aDirectory
}

$FMU_DIR = "CtrlProactive"

createEmptyDir($FMU_DIR)
createEmptyDir("$FMU_DIR/sources")
createEmptyDir("$FMU_DIR/binaries/win64")
createEmptyDir("$FMU_DIR/resources")

copy src-gen/*.cpp $FMU_DIR/sources
copy src-gen/*.h $FMU_DIR/sources

copy "lib$FMU_DIR.dll" $FMU_DIR/binaries/win64/$FMU_DIR.dll
copy C:\msys64\mingw64\bin\libwinpthread-1.dll $FMU_DIR/binaries/win64/

copy src-gen/modelDescription.xml $FMU_DIR/

copy *.fmu $FMU_DIR/resources

Push-Location $FMU_DIR

create-zip "." "$FMU_DIR.fmu"

Pop-Location

pause
