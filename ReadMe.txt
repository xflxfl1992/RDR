1. click on XFL_LidarProc_Windows.sln in the fold 02_XFL_Lidar_Proc_DLG.
2. run ListBmpFileName.bat in the fold original_data to regenerate binFileNameList.txt.
3. copy your own path to binFileNameList.txt, and change the original path "sprintf_s(filename1, "C:\\Users\\LeiLei\\Desktop\\第二篇\\非结构化道路\\ori\\binFileNameList.txt");" in line 336 of XFL_LidarProc_WindowsDlg.cpp
4. run the code to see the outcomes.
PS：You can get intermediate result by change the configures in XFL_Lidar64ProcFrame.cpp. Remember to create a new folder named "tmp" in your D disk.
