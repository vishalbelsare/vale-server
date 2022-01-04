import os
import shutil

DEPS = {
    "C:\\Qt\\6.1.2\\mingw81_64\\bin": [
        "LIBWINPTHREAD-1.DLL",
        "LIBGCC_S_seh-1.DLL",
        "LIBSTDC++-6.DLL",

        "Qt6Svg.dll",
        "Qt6Gui.dll",
        "Qt6Core.dll",
        "Qt6Network.dll",
        "Qt6Widgets.dll",

        "libGLESV2.dll",
        "libEGL.dll",

        # HTTPS stuff
        "libeay32.dll",
        "ssleay32.dll",

        "D3Dcompiler_47.dll",

        "WinSparkle.dll",
        "WinSparkle.lib",

        "TurboActivate.dll",
        "TurboActivate.lib"
    ],
    "C:\\Users\\jdkato\\Desktop\\Git\\build-systray-Desktop_Qt_6_1_2_MinGW_64_bit-Release\\release": [
        "styles",
        "platforms",
        #"bearer",
        "imageformats",
        "iconengines",

        # TODO: Do we really need this?
        "translations",

        "Vale Server.exe",
        "dashboard",
        "valelib.exe",
        "vale.exe"
    ]

}


def get_packages():
    """Copy all required packages from the release directory.
    """
    dst = os.path.abspath("Vale Server")

    # Clean up old data
    if os.path.isdir(dst):
        shutil.rmtree(dst)
    os.makedirs(dst)

    for src, deps in DEPS.items():
        for dep in deps:
            dep = os.path.join(src, dep)
            if os.path.isdir(dep):
                loc = os.path.join(dst, os.path.basename(dep))
                shutil.copytree(dep, loc)
            else:
                shutil.copy(dep, dst)


if __name__ == '__main__':
    get_packages()
