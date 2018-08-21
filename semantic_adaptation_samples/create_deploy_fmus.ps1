Push-Location CtrlMultiRate
& .\create_fmu.ps1
copy CtrlMultiRate\CtrlMultiRate.fmu "C:\Users\clagm\Dropbox\Mehrdad and Claudio\MasterAlgorithms\FMPy_fixed_step_fmu_case\" -Force
Pop-Location

Push-Location PlantMultiRate
& .\create_fmu.ps1
copy PlantMultiRate\PlantMultiRate.fmu "C:\Users\clagm\Dropbox\Mehrdad and Claudio\MasterAlgorithms\FMPy_fixed_step_fmu_case\" -Force
Pop-Location

Push-Location SensorMultiRate
& .\create_fmu.ps1
copy SensorMultiRate\SensorMultiRate.fmu "C:\Users\clagm\Dropbox\Mehrdad and Claudio\MasterAlgorithms\FMPy_fixed_step_fmu_case\" -Force
Pop-Location

Push-Location SensorPlantMultiRate
& .\create_fmu.ps1
copy SensorPlantMultiRate\SensorPlantMultiRate.fmu "C:\Users\clagm\Dropbox\Mehrdad and Claudio\MasterAlgorithms\FMPy_fixed_step_fmu_case\" -Force
Pop-Location

