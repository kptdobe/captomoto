import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { Observable } from 'rxjs';
import { AuthService } from '../../services/auth.service';
import { AngularFireDatabase } from 'angularfire2/database';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  front: Observable<any>;
  pool: Observable<any>;

  displayName: String;

  constructor(public navCtrl: NavController, private auth: AuthService, private db: AngularFireDatabase) {
    this.auth.afAuth.authState
      .subscribe(
        user => {
          this.displayName = this.auth.getDisplayName();
        }
      );

    this.front = this.db.object('/sensors/front').valueChanges();
    this.pool = this.db.object('/sensors/pool').valueChanges();

  }

  toggleLight(isChecked, item) {
    this.db.object('/sensors/' + item).update({ light: isChecked });
  }

}
