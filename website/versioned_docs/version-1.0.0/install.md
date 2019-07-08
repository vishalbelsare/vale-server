---
id: version-1.0.0-install
title: Installation
sidebar_label: Installation
original_id: install
---

Vale Server is a commercial desktop application available on macOS and Windows.

See the Vale Server [homepage](https://errata.ai/vale-server/) to learn more.

## macOS

### Installation

:::important
Vale Server requires at least macOS Sierra 10.12.
:::

Double-click `vs-setup-x64-mac.dmg` to open the installer, then drag the application
to the Applications folder.

![Vale Server DMG](assets/ui/macOS/dmg.png)

When you open the application, Vale Server will launch in the background at
`http://localhost:7777` (by default) and you will see its icon in your menu
bar:

![Vale Server menu icon](assets/ui/macOS/menu.png)

Clicking this icon will open a menu that allows you to
interact with the server.

### Uninstallation

1. Select **Quit Vale Server** from the menu bar:

    <img src="assets/ui/macOS/quit.png" class="small">

2. Open your Applications folder and drag the Vale Server application to the Trash.

3. Empty the Trash (optional).

## Windows

### Installation

:::important
Vale Server supports Windows 7, 8.1, and 10.
:::

:::important
If you're warned about Vale Server being an "unrecognized app," you can
click "More info" and then "Run anyway."

![A screenshot of Windows "unrecognized app" warning](assets/ui/Windows/security.png)
:::

Download either `vs-setup-x64-windows.exe` (64-bit systems) or `vs-setup-x86-windows.exe`
(32-bit systems) and follow the on-screen instructions:

![A screenshot of Vale Server's Windows installer](assets/ui/Windows/install.png)

After completing the installation wizard, Vale Server will be installed at
`C:\Program Files\Vale Server` (64-bit) or `C:\Program Files (x86)\Vale Server`
(32-bit).

To start the application click `Vale Server.exe` from the installation
location. Vale Server will launch in the background at `http://localhost:7777`
(by default) and you will see its icon in your menu bar:

![A screenshot highlighting Vale Server's icon in the taskbar](assets/ui/Windows/taskbar.png)

### Uninstallation

Navigate to your Vale Server installation folder
(`C:\Program Files\Vale Server` or `C:\Program Files (x86)\Vale Server`),
click `maintenancetool.exe`, and select "Remove all components":

![A screenshot of Vale Server's maintenance tool](assets/ui/Windows/uninstall.png)

## Ubuntu

Coming soon.
