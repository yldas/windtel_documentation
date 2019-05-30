import { Component } from '@angular/core';

import { Experiment } from '../experiment';
import { UserService } from '../user.service';
import { User } from '../user';
import { Esp32Service } from "../esp32.service";

@Component({
  selector: 'app-tab2',
  templateUrl: 'tab2.page.html',
  styleUrls: ['tab2.page.scss']
})
export class Tab2Page {
  // Form control variables
  numPressureSensors: number = 16;
  pressureCheck: boolean = false;
  allPressures: boolean = false;
  forcesCheck: boolean = false;
  allForces: boolean = false;
  humidityCheck: boolean = false;
  temperatureCheck: boolean = false;
  speedCheck: boolean = false;

  // Page control variables
  currentUser: User;
  permissions: string;
  success: boolean = false;

  // Message variables
  rodLength: number = 15;
  expDuration: number = 60;
  pressureSamples: number = 0;
  pressureSensors: boolean[] = new Array(16).fill(false);
  forcesSamples: number = 0;
  forcesSensors: boolean[] = new Array(5).fill(false);
  humSamples: number = 0;
  tempSamples: number = 0;
  windSpeedSamples: number = 0;

  constructor(
    private userService: UserService,
    private esp32Service: Esp32Service) {
      esp32Service.messages.subscribe(msg => {
        console.log("Response from websocket: " + msg);
      });
    }

  tryExperiment() {
    this.sendMsg();
  }

  getCurrentUser() {
    this.currentUser = this.userService.getCurrentUser();
    this.permissions = this.currentUser.rpermissions;
  }

  tryExperimentStart() {
    // TODO
  }

  ngOnInit() { }

  sendMsg() {
    let message = {
      rodLength: this.rodLength,
      expDuration: this.expDuration,
      pressureSamples: this.pressureSamples,
      pressureSensors: this.pressureSensors,
      forcesSamples: this.forcesSamples,
      forcesSensors: this.forcesSensors,
      humSamples: this.humSamples,
      tempSamples: this.tempSamples,
      windSpeedSamples: this.windSpeedSamples
    }
    console.log("New message from WindTel App to WebSocket: ", message);
    this.esp32Service.messages.next(message);
  }

}
