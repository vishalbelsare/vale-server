# Get Started with Vale Server for Mac

Vale Server is a standalone desktop application for [Vale][1], a linter for
prose that emphasizes performance, privacy, and extensibility.

```callout{'title': 'NOTE', 'classes': ['tip']}
Vale Server requires at least macOS Sierra 10.12.
```

## Installation

Double-click `Vale Server.dmg` to open the installer, then drag the application
to the Applications folder.

<img alt="Vale Server" src="img/ui/macOS/dmg.png" style="width: 75%; display: block; margin: 0 auto;">

When you open the application, Vale Server will launch in the background at
`http://localhost:7777` (by default) and you will see its icon (<img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">)
in your menu bar:

<img alt="Tray icons" src="img/ui/macOS/menu.png" style="width: 100%; display: block; margin: 0 auto;">

Clicking this icon will open a menu that allows you to
interact with the server.

## Preferences

Choose <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Preferences** from the menu bar and configure the run-time options described
below.

### General

<img alt="Vale Server Preferences (General)" src="img/ui/macOS/prefs-general.png" style="width: 75%; display: block; margin: 0 auto;">

The available options are:

- **Enter your License Key**: In order to use Vale Server, you need to enter a
  valid license key. You'll receive this key in your purchase confirmation
  email. If your key is valid, you'll see the following pop-up dialog:

  <img alt="Key Dialog" src="img/ui/macOS/key-dialog.png" style="width: 70%; display: block; margin: 0 auto;">

- **Start Vale Server when you log in**: Check this option if you want Vale
  Server to start when you log in to your Mac.

### Projects

Projects allow you to manage multiple [Vale configuration files](https://errata-ai.github.io/vale/config/)
in one place. Each project has a name (the left panel) and an associated
configuration file (the right panel):

<img alt="Vale Server Preferences (Projects)" src="img/ui/macOS/prefs-projects.png" style="width: 75%; display: block; margin: 0 auto;">

To create a new Project, click "Add" and then double-click the new project
to give it a name:

<img alt="Vale Server Preferences (New Project)" src="img/ui/macOS/new-project.png" style="width: 75%; display: block; margin: 0 auto;">

You can now edit the configuration file on the right to customize the new
project. To remove a project, click its name and then click the "Remove"
button.

After you've created and configured your projects, you can quickly switch
between them by selecting <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Select Project**:

<img alt="Vale Server Preferences (Select Project)" src="img/ui/macOS/select-project.png" style="width: 50%; display: block; margin: 0 auto;">

Additionally, each project has an associated Vocabulary that you can manage
via the [Dashboard](https://errata-ai.github.io/vale-server/#dashboard).

### Styles

The Styles page provides quick access to all of your installed styles. You can add your
own styles by selecting <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Browse Styles...** and then copying the relevant files.

<img alt="Vale Server Preferences (Styles)" src="img/ui/macOS/prefs-styles.png" style="width: 75%; display: block; margin: 0 auto;">

Vale Server ships with built-in styles for some of the most popular
open-source writing tools and style guides:

- **Joblint** [[source](https://github.com/rowanmanning/joblint), [demo](https://joblint.org/)]:

    > Test tech job posts for issues with sexism, culture, expectations, and recruiter fails.
    >
    > Writing a job post? Use Joblint to make your job attractive to a much broader range of candidates and ensure you're not being discriminatory.
    >
    > Getting swamped in job posts? Use Joblint to filter out the bad ones.

- **Microsoft** [[source](https://docs.microsoft.com/en-us/style-guide/welcome/), [documentation](https://github.com/errata-ai/Microsoft)]:

    > A Vale-compatible implementation of the Microsoft Writing Style Guide.

- **Vale** [[source](https://github.com/errata-ai/vale), [documentation](https://errata-ai.github.io/vale/)]:

    > The Vale style implements a spell checker that respects your custom [Vocabulary](https://errata-ai.github.io/vale-server/#dashboard).

- **proselint** [[source](https://github.com/amperser/proselint), [documentation](http://proselint.com/)]:

    > `proselint` places the world’s greatest writers and editors by your side,
    > where they whisper suggestions on how to improve your prose.

- **write-good** [[source](https://github.com/btford/write-good)]:

    > Naive linter for English prose for developers who can't write good and
    > wanna learn to do other stuff good too.

### Advanced

The Advanced page allows you to configured the port that Vale Server
will listen on:

<img alt="Vale Server Preferences (Advanced)" src="img/ui/macOS/prefs-advanced.png" style="width: 75%; display: block; margin: 0 auto;">

## Dashboard

The Vale Dashboard allows you to manage a custom Vocabulary for each of your
Projects. To open your dashboard, select <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Open Dashboard...** from the menu bar.

<img alt="Vale Server Preferences (Advanced)" src="img/ui/macOS/dashboard.png" style="width: 75%; display: block; margin: 0 auto;">

A *Vocabulary* consists of terms and phrases that belong to one of two groups:

- **Preferred**: Entries marked as "Preferred" will be enforced on as-entered
  basis&mdash;for example, if the term "JavaScript" is marked as Preferred, any
  variation that doesn't exactly match (e.g., "Javascript", "javascript", etc.) will
  raise an error. Additionally, any Preferred entries will automatically be
  added as exceptions to `spelling`, `capitalization`, and `conditional` rules.

- **Do not use**: Any use of an entry marked as "Do not use" will raise an error.

To manually edit a Vocabulary file, choose <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Browse Styles...** from the menu
bar and then navigate to the `/Vocab/<project name>` directory.

## Clipboard

Vale Server has the ability to check the contents of your clipboard against the
current Project's configuration, allowing it to be used from any application or
website. To check your clipboard, choose <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Check Clipboard as...** from the menu
bar:

<img alt="Vale Server (Check Clipboard)" src="img/ui/macOS/clipboard.png" style="width: 50%; display: block; margin: 0 auto;">

## Uninstallation

1. Select <img src="img/menu-icon.png" style="height: 18px; padding-left: 2px; padding-right: 2px">
<i class="fas fa-long-arrow-alt-right"></i> **Quit Vale Server**:

    <img alt="Vale Server Quit" src="img/ui/macOS/quit.png" style="width: 50%; display: block; margin: 0 auto;">

2. Open your Applications folder and drag the Vale Server application to the Trash.

3. Empty the Trash (optional).

[1]: https://github.com/errata-ai/vale
