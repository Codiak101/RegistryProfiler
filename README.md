# Registry Profiler
![image](https://user-images.githubusercontent.com/78706679/148661069-bbf8affd-a151-40e5-a9c5-9ef5f0e2fe76.png)

This app allows certain registry values like the ones for brightness & volume to change and saved into profiles. The first menu gives the options to manage the profiles stored in a created file. When the app is first launched it saves a default profile of the device's current values along with any others later created. The horizontal menu gives the registry categories each containing the settings, some are checkboxes and others are number entries.

## Requirements
![image](https://user-images.githubusercontent.com/78706679/148661904-d7723676-ddfd-4643-b887-8e708d270714.png)
- A hacked device - either phat OLED model, slim LED model or PSTV
- An app to install [Released VPK](<https://github.com/Codiak101/RegistryProfiler/releases/latest>)

## Controls
![image](https://user-images.githubusercontent.com/78706679/148660497-e6c4ee3d-8289-4963-971d-70a3bd2a642d.png)
- L1/R1 - Navigate horizontal menu
- Up/Down - Option selection (menu item, registry setting or profile char)
- Left/Right - Registry value change (hold for speed) or profile/char item selection
- Circle - Back/Cancel to last page or decrement by snap value (hold for min value)
- Cross - Enter menu option, confirm profile action or increment by snap value (hold for max value)
- Square - Refresh registry values, snap back to Profile menu or hold to reset to when app first opened
- Triangle - Exit app from anywhere

## Profile Menu
![image](https://user-images.githubusercontent.com/78706679/148660780-5d4c54c8-5b14-444b-890b-dd9723f7cb9e.png)
- SAVE[F] - Full profile with all registries saved and is always active, allows for a max of 12 profiles with 4 character names
- SAVE[C] - Changes only profile with those registries saved, saves into same max of 12 profiles and is only active with registry changes
- LOAD - Load one of the saved profiles, can see the saved values on the confirmation page and can only select slots that have a profile
- DELETE - Delete one of the saved profiles when there's more than one, can see the saved values on the confirmation page and can only select slots that have a profile
- CONTROLS - As listed in section above
- REBOOT - Reboot device to apply registry updates when one or more settings updated
- EXIT - Exit app and see credits

## Miscellaneous
![image](https://user-images.githubusercontent.com/78706679/148661092-396ac288-5f14-423a-af20-41727b8c732e.png)
- Last action is saved on the banner which is either a registry change, profile action or a reset
- Profiles saved in ux0:data/PSREGPROF/profiles.txt
- Internal app images saved in ux0:app/PSREGPROF
- Live area has links to Vita and PSTV user guides and this github

## Credits
![image](https://user-images.githubusercontent.com/78706679/148661402-9b929a65-c385-47f5-85a0-639e78f3b968.png)

https://github.com/xerpi/libvita2d

https://github.com/kprinssu/vitamenulib

https://github.com/devnoname120/RegistryEditorMOD

https://github.com/CallmeBK/VitaBatteryAlarm
