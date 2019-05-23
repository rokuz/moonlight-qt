TEMPLATE = subdirs
SUBDIRS = \
    moonlight-common-c \
    qmdnsengine \
    app \
    h264bitstream

# Build the dependencies in parallel before the final app
app.depends = qmdnsengine moonlight-common-c h264bitstream
win32:!winrt {
    SUBDIRS += AntiHooking
    app.depends += AntiHooking
}
!winrt {
    SUBDIRS += soundio
    app.depends += soundio
}

# Support debug and release builds from command line for CI
#CONFIG += debug_and_release
CONFIG += app_bundle

# Run our compile tests
load(configure)
qtCompileTest(SL)
