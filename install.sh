
PLIST="com.ben.driveutil.plist"
USER=$(id -u)

# validate using plutil

sudo cp $PLIST /Users/bp/Library/LaunchAgents
launchctl bootout gui/$USER/com.dril.agent
launchctl bootstrap gui/$USER /Users/bp/Library/LaunchAgents/com.ben.driveutil.plist
