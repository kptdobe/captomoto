import { Injectable } from '@angular/core';
import { AngularFireAuth } from 'angularfire2/auth';
import * as firebase from 'firebase/app';

@Injectable()
export class AuthService {
    private user: firebase.User;

    constructor(public afAuth: AngularFireAuth) {
        afAuth.authState.subscribe(user => {
            console.log('Authstate changed!', user);
            this.user = user;
        });
    }


    get authenticated(): boolean {
        return this.user !== null;
    }

    getEmail() {
        return this.user && this.user.email;
    }

    getDisplayName() {
        return this.user && this.user.email;
    }

    signOut(): Promise<void> {
        return this.afAuth.auth.signOut();
    }

    signInWithEmail(credentials) {
        return this.afAuth.auth.signInWithEmailAndPassword(credentials.email, credentials.password);
    }
}