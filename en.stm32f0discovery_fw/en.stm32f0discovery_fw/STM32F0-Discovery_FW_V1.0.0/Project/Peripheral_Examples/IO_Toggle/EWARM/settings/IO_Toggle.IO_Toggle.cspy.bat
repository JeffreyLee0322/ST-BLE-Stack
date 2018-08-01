@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


@echo off 

if not "%~1" == "" goto debugFile 

@echo on 

"F:\CC3200\iar\common\bin\cspybat" -f "E:\CMS\STM32_discovery\en.stm32f0discovery_fw\STM32F0-Discovery_FW_V1.0.0\Project\Peripheral_Examples\IO_Toggle\EWARM\settings\IO_Toggle.IO_Toggle.general.xcl" --backend -f "E:\CMS\STM32_discovery\en.stm32f0discovery_fw\STM32F0-Discovery_FW_V1.0.0\Project\Peripheral_Examples\IO_Toggle\EWARM\settings\IO_Toggle.IO_Toggle.driver.xcl" 

@echo off 
goto end 

:debugFile 

@echo on 

"F:\CC3200\iar\common\bin\cspybat" -f "E:\CMS\STM32_discovery\en.stm32f0discovery_fw\STM32F0-Discovery_FW_V1.0.0\Project\Peripheral_Examples\IO_Toggle\EWARM\settings\IO_Toggle.IO_Toggle.general.xcl" "--debug_file=%~1" --backend -f "E:\CMS\STM32_discovery\en.stm32f0discovery_fw\STM32F0-Discovery_FW_V1.0.0\Project\Peripheral_Examples\IO_Toggle\EWARM\settings\IO_Toggle.IO_Toggle.driver.xcl" 

@echo off 
:end