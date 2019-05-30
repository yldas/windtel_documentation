import { IonicModule } from '@ionic/angular';
import { RouterModule } from '@angular/router';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Tab1Page } from './tab1.page';
import { MessagesComponent } from '../messages/messages.component';

@NgModule({
  declarations: [
    Tab1Page,
    MessagesComponent
  ],
  imports: [
    IonicModule,
    CommonModule,
    RouterModule.forChild([{ path: '', component: Tab1Page }])
  ]
})
export class Tab1PageModule { }
