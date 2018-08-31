import { Component } from '@angular/core';
import { IonicPage } from 'ionic-angular';
import { AngularFireDatabase, AngularFireObject } from 'angularfire2/database';
import { Observable } from 'rxjs';

@IonicPage()
@Component({
  selector: 'page-front',
  templateUrl: 'front.html',
})
export class FrontPage {
  frontRef: AngularFireObject<any>;
  front: Observable<any>;

  constructor(private db: AngularFireDatabase) {
    this.frontRef = this.db.object('/sensors/front');
    this.front = this.frontRef.valueChanges();
  }

  toggleLight(isChecked) {
    this.frontRef.update({light: isChecked});
  }
}
