To run the project, dll files in the "dll" folder are required.

Build the project in "Release mode" and add dll files to the "x64/Release" folder.

After completing this process, your project will run.

* All settings (library paths, etc.) for the project are stored in Release mode, so it is recommended to build in Release mode.
  When building in Debug mode, you must set all necessary settings and library paths.

* When distributing an executable file, it must be distributed along with the "SDKResource" folder and other necessary resources. 
  The "SDKResource" folder contains files required to run mata_SDK program.