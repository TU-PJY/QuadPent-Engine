== Running project ==
To run the project, dll files in the "dll" folder are required.
Build the project in "Release mode" and add dll files to the "x64/Release" folder.
After completing this process, your project will run.
  
  
== Adding source code ==
If you add a mode code, add the code to "WorkSpace\Mode".
If you add a Object code, add the code to "WorkSpace\Object".


== Updating QuadPent version ==
If the version of the QuadPent that your previous project using is 8.1 or higher,
you just need to copy the "WorkSpace" folder inside the higher version "QuadPent_Engine" folder and then proceed with the "Running Project" step.

If the version is 8.0 or lower, 
You will need to copy your previously worked code to "WorkSpace" and do separate work to make it compatible with the current version.


== CAUTION ==
- Do not refactor engine system functions and variable names, as this may cause problems in future engine version updates.
- The glsl code can be modified, but when updating the engine version, the corresponding glsl code must also be copied.
- It is recommended that file passwords used in a project be stored separately, outside the project.
- Be especially careful not to lose any file passwords used in your project.
- All settings (library paths, etc.) for the project are stored in Release mode, so it is recommended to build in Release mode.
  When building in Debug mode, you must set all necessary settings and library paths.
- When distributing an executable file, it must be distributed along with the "SystemComponent" folder and other necessary resources. 
  The "SystemComponent" folder contains files required to run QuadPent program.