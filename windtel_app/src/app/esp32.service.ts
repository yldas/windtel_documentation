import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { map } from 'rxjs/operators';
import { WebsocketService } from "./websocket.service";

const ESP32_URL = "ws://192.168.43.5/";

export interface expMessage {
  rodLength: number,
  expDuration: number,
  pressureSamples: number,
  pressureSensors: boolean[],
  forcesSamples: number,
  forcesSensors: boolean[],
  humSamples: number;
  tempSamples: number,
  windSpeedSamples: number
}

export interface Message {
  sender: string,
  message: string
}

@Injectable({
  providedIn: 'root'
})
export class Esp32Service {
  constructor(wsService: WebsocketService) {
    this.messages = <Subject<any>>wsService.connect(ESP32_URL).pipe(map(
      (response: MessageEvent): any=> {
        let data = JSON.parse(response.data);
        console.log(data);
        /*
        console.log(data.author);
        console.log(data.message);
        */
        return {
          sender: data.sender,
          message: data.message
        };
      }
    ));
  }

  public tryConnect() {
    //
  }

  public messages: Subject<any>;
}
