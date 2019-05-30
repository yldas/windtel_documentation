import { Component } from '@angular/core';

import { UserService } from '../user.service';
import { User } from '../user';

import { ResultsService } from '../results.service';
import { Data } from '../Data'; 
import { Measurement } from '../measurement';
import { Chart } from 'chart.js';

@Component({
  selector: 'app-tab3',
  templateUrl: 'tab3.page.html',
  styleUrls: ['tab3.page.scss']
})
export class Tab3Page {
  measurements: Measurement[];
  data: Data[];
  windSpeeds = []
  windSpeed_ts = [];
  temperatures = [];
  temperature_ts = [];
  humidities = [];
  humidity_ts = [];

  sampleNum = [];

  month = [];
  price = [];
  chart = [];

  currentUser: User;
  permissions: string;
  success: boolean = false;

  tryResults() {
    this.success = true;
  }

  getMeasurements() {
    this.resultsService.getResultsFromDatabase().subscribe((res: Measurement[]) => {
      let meas = [];
      res.forEach(measurement => {
        meas.push(measurement);
        if (measurement.windSpeed) {
          this.windSpeeds.push(measurement.windSpeed);
        }
        if (measurement.windSpeed_t) {
          this.windSpeed_ts.push(measurement.windSpeed_t);
        }
        if (measurement.temperature) {
          this.temperatures.push(measurement.temperature);
        }
        if (measurement.temperature_t) {
          this.temperature_ts.push(measurement.temperature_t);
        }
        if (measurement.humidity) {
          this.humidities.push(measurement.humidity);
        }
        if (measurement.humidity_t) {
          this.humidity_ts.push(measurement.humidity_t);
        }
      });
      this.measurements = meas;
      var i;
      for (i = 1; i < this.measurements.length; i++) {
        this.sampleNum.push(i);
      }
      console.log(this.measurements);
    })
  }

  plotSpeedLine() {
    this.chart = new Chart('canvas', {
      type: 'line',
      data: {
        datasets: [{
          label: 'Wind Speed vs Time',
          data: this.windSpeeds
        }],
        labels: this.windSpeed_ts
      }
    });
  }

  plotSpeed() {
    let zip = new Array();
    for (var i = 0; i < this.windSpeed_ts.length; i++) {
      zip.push([this.windSpeed_ts[i], this.windSpeeds[i]]);
    }
    
    console.log(zip);
    let dataPoints = [];
    zip.forEach(point => {
      dataPoints.push(
        {
          x: point[0],
          y: point[1]
        }
      )
    })
    console.log(dataPoints);
    this.chart = new Chart('canvas', {
      type: 'scatter',
      data: {
        datasets: [{
          label: 'Wind Speed Time Series',
          data: dataPoints,
          borderColor: '#3880ff',
          showLine: true,
          fill: false
        }]
      },
      options: {
        fill: false,
        scales: {
          xAxes: [{
            type: 'linear',
            position: 'bottom'
          }]
        }
      }
    });
  }

  getResults() {
    this.resultsService.getResultsFromDatabase().subscribe((res: Data[]) => {
      console.log(res);
      res.forEach(y => {
        this.month.push(y.month);
        this.price.push(y.price);
      });
      this.chart = new Chart('canvas', {
        type: 'line',
        data: {
          labels: this.month,
          datasets: [
            {
              data: this.price,
              borderColor: '#3cba9f',
              fill: false
            }
          ]
        },
        options: {
          legend: {
            display: false
          },
          scales: {
            xAxes: [{
              display: true
            }],
            yAxes: [{
              display: true
            }],
          }
        }
      });
    });
  }
  
  constructor(private resultsService: ResultsService) { }

  ngOnInit() {
    //this.getMeasurements();
  }
}
