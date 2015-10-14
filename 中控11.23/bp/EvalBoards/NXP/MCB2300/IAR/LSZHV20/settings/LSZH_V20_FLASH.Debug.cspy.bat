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


"D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\common\bin\cspybat" "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\bin\armproc.dll" "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\bin\armjlink.dll"  %1 --plugin "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\bin\armbat.dll" --device_macro "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\config\debugger\NXP\LPC23xx.dmac" --flash_loader "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\config\flashloader\NXP\FlashNXPLPC512K2.board" --backend -B "--endian=little" "--cpu=ARM7TDMI-S" "--fpu=None" "-p" "D:\program files (x86)\IAR Systems\Embedded Workbench 7.0\arm\CONFIG\debugger\NXP\LPC2368.ddf" "--drv_verify_download" "--semihosting" "--device=LPC2368" "--jlink_device_select=0" "--drv_communication=USB0" "--drv_interface_speed=auto" "--jlink_initial_speed=32" "--drv_restore_breakpoints=main" 


