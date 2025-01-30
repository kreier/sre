# Scaled Resolution Editor (SRE)

Scaled Resolution Editor (SRE) is a tool for editing the list of GPU-scaled resolutions that are automatically added by the graphics driver. For AMD GPUs, it can also add custom VSR resolutions.

This is just a fork of the work by [ToastyX](https://www.monitortests.com/forum/User-ToastyX) has his first blog post about this in February 2022. Please download the latest version 1.0 from 2024-01-15 directly from [https://www.monitortests.com/forum/Thread-Scaled-Resolution-Editor-SRE](https://www.monitortests.com/forum/Thread-Scaled-Resolution-Editor-SRE) and consider supporting him [on Patreon](https://www.patreon.com/ToastyX).

[![Patreon support](docs/patreon.png)](https://www.patreon.com/ToastyX)

### Requirements:

- Windows Vista or later
- AMD/ATI or NVIDIA GPU with appropriate driver installed (Intel is not supported)
Getting started:
Run SRE.exe. A UAC prompt may appear because it needs permission to access the registry.
Select a GPU from the drop-down list. The first active GPU is selected automatically. "*" means changes were saved.
Edit the resolution list as desired.
Click "OK" to save the changes.
Reboot (or run restart.exe from CRU).
Adding resolutions should be safe because the resolutions are scaled by the GPU, and deleting resolutions should be safe because the resolutions defined by the monitor will always be available. If something goes wrong, run SRE after booting Windows in safe mode and use the "Default" button to restore the default resolution list.

Upgrading the graphics driver may reset the resolution list. You can use the "Export" button to save the resolutions to a text file that you can import later.

AMD-specific notes:
Resolutions greater than the monitor's native resolution will only be available if Virtual Super Resolution (VSR) is enabled and the aspect ratio matches the native resolution. The highest possible resolution will depend on the GPU.
The driver automatically adds certain resolutions by default only if the aspect ratio matches the native resolution. These are listed in gray. If the resolution is not available but you want it to be, add the resolution to enable it. If you don't want the resolution, you can delete it.
3200x1800 is hard-coded in the driver and can't be deleted. 1920x1080 and 1280x720 are hard-coded in the driver for 21:9 displays. Let me know if you find any other exceptions.
NVIDIA-specific notes:
Resolutions greater than the monitor's native resolution will not be available.
1768x992 and 1176x664 are hard-coded in the driver and can't be deleted. Let me know if you find any other exceptions.
Changes in 1.0:
Fixed existing resolutions not being found with certain drivers (Lenovo Legion Go)
Added new default 21:9 resolutions for AMD
Changes in Beta 4:
Added new default resolutions for AMD: 1128x634, 1760x990, 1600x1000, 3072x1728
Changes in Beta 3:
Fixed failure to start on older versions of Windows 10
Changes in Beta 2:
Fixed failure to start on older versions of Windows
Fixed 1x1 appearing in NVIDIA control panel on some displays if all resolutions are deleted
