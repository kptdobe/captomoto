const functions = require('firebase-functions');

const admin = require('firebase-admin');
admin.initializeApp(functions.config().firebase);

exports.onFrontLightOn = functions.database.ref('/sensors/front/light')
    .onWrite((change, context) => {
        // Grab the current value of what was written to the Realtime Database.
        const newValue = change.after.val();
        console.log(`Detected an incoming new value for the front light: ${newValue}`);
        if (newValue === true) {
            console.log('Setting a new timer');
            
            return admin.database().ref('/sensors/front/light_timer')
                .set(new Date().getTime())
                .catch(err => {
                    console.error('Error while setting the timer', err);
                });
        } else {
            console.log('Removing the timer');
            return admin.database().ref('/sensors/front/light_timer')
              .remove()
              .catch(err => {
                console.error('Error while removing the timer', err);
              });
        }
});

// called by https://cron-job.org/en/members/jobs/details/?jobid=2518853
exports.checkFrontLight = functions.https.onRequest(async (req, res) => {
    const snapshot = await admin.database().ref('/sensors/front').once('value');
    const timer = snapshot.val().light_timer;
    const DURATION = snapshot.val().DURATION;

    console.log(`Expected duration: ${DURATION} mins`);
    if (timer) {
        const now = new Date().getTime();
        console.log(`Ellapsed time: ${(now - timer) / (1000 * 60)} mins.`);
        if (now - timer > (DURATION * 60 * 1000)) {
            // turn light off
            console.log('Checker is turning off the light now');
            await admin.database().ref('/sensors/front/light').set(false);
            return res.status(202).end();
        }
    }
    return res.status(204).end();
});
