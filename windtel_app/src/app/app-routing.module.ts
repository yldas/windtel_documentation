import { NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';

const routes: Routes = [
  { path: 'tabs',
    pathMatch: 'full',
    loadChildren: './tabs/tabs.module#TabsPageModule',
  },
  { path: '',
    pathMatch: 'full',
    loadChildren: './login/login.module#LoginPageModule'
  },
  { path: 'signup', loadChildren: './signup/signup.module#SignupPageModule' },
  { path: 'experiment', loadChildren: './experiment/experiment.module#ExperimentPageModule' },
  { path: 'settings', loadChildren: './settings/settings.module#SettingsPageModule' },
  { path: 'manage-users', loadChildren: './manage-users/manage-users.module#ManageUsersPageModule' },
  { path: 'diagnostics', loadChildren: './diagnostics/diagnostics.module#DiagnosticsPageModule' }
];
@NgModule({
  imports: [
    RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules })
  ],
  exports: [RouterModule]
})
export class AppRoutingModule {}
