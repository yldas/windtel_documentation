import { IonicModule } from '@ionic/angular';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { TabsPageRoutingModule } from './tabs.router.module';

import { TabsPage } from './tabs.page';

@NgModule({
  declarations: [
    TabsPage
  ],
  imports: [
    IonicModule,
    CommonModule,
    TabsPageRoutingModule
  ]
})
export class TabsPageModule {}
