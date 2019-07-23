try {
    var addon = require("./index.js");
}
catch (e) {
    console.log(`Failed to load with exception ${e}`);
}

const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('Press enter to exit\n\n', (answer) => {
    addon.logInfoEvent("Finished!");
    rl.close();
});

addon.logEvent("About to run an activity");
addon.logStartCommand("init", "starting to run");
addon.logEvent("this is a test");
addon.logStartUpdateProgram("program changed");
addon.logStopUpdateProgram("program updated");
addon.logPerfEvent("Boy that went really fast - 0.1ms!");
addon.logStopCommand("init", "OK, we're done here");
