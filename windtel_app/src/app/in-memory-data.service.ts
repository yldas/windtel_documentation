import { InMemoryDbService } from 'angular-in-memory-web-api';
import { User } from './user';
import { Experiment } from './experiment';
import { Measurement } from './measurement';
import { PressurePoint } from './pressure-point';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class InMemoryDataService implements InMemoryDbService {
  createDb() {
    const users = [
      { rid: 1, rname: 'Misael', rlast_name: 'Valentin', remail: 'misael.valentin@upr.edu', rvocation: 'Student', rdepartment: 'ICOM', rinstitution: 'UPRM', rcity: 'Aguadilla', rpassword: '12345678', rpermissions: 'Researcher' },
      { rid: 5, rname: 'Admin', rlast_name: 'Zapata', remail: 'admin@upr.edu', rvocation: 'Professor', rdepartment: 'INCI', rinstitution: 'UPRM', rcity: 'San Juan', rpassword: 'adminpass', rpermissions: 'Administrator' }
    ];

    const experiments = [
      { eid: 1, ename: 'my_first_experiment', edescription: 'This was my first experiment!', etime: '5 minutes', edate: '4/1/2019', rid: 1 }
    ];

    const measurements = [
      { mid: 1, eid: 1, temperature: '25.02', temperature_t: '0', humidity: '85.09', humidity_t: '0', windspeed: '30.50', windspeed_t: '0',
        dragfrontforce: '8.0', dragfrontforce_t: '0', dragbackforce: '7.0', dragbackforce_t: '0', rightsideforce: '1.1', rightsideforce_t: '0', leftsideforce: '2.3', leftsideforce_t: '0', liftforce: '15.0', liftforce_t: '0'
      },
      { mid: 2, eid: 1, temperature: '25.02', temperature_t: '3', humidity: '85.09', humidity_t: '3', windspeed: '25.33', windSpeed_t: '3', 
        dragfrontforce: '8.0', dragfrontforce_t: '2', dragbackforce: '7.5', dragbackforce_t: '2', rightsideforce: '1.5', rightsideforce_t: '2', leftsideforce: '2.0', leftsideforce_t: '2', liftforce: '15.0', liftforce_t: '2'
      },
      { mid: 3, eid: 1, temperature: '25.22', temperature_t: '5', humidity: '85.10', humidity_t: '5', windspeed: '24.00', windSpeed_t: '5', 
        dragfrontforce: '8.0', dragfrontforce_t: '4', dragbackforce: '7.5', dragbackforce_t: '4', rightsideforce: '1.3', rightsideforce_t: '4', leftsideforce: '2.5', leftsideforce_t: '4', liftforce: '15.0', liftforce_t: '4'
      },
      { mid: 4, eid: 1, temperature: '26.03', temperature_t: '10', humidity: '88.99', humidity_t: '10', windSpeed: '21.32', windSpeed_t: '10', 
        dragfrontforce: '8.0', dragfrontforce_t: '6', dragbackforce: '7.0', dragbackforce_t: '6', rightsideforce: '1.0', rightsideforce_t: '6', leftsideforce: '2.3', leftsideforce_t: '6', liftforce: '20.3', liftforce_t: '6'
      },
      { mid: 5, eid: 1, temperature: '26.03', temperature_t: '15', humidity: '88.99', humidity_t: '15', windSpeed: '20.32', windSpeed_t: '13', 
        dragfrontforce: '8.0', dragfrontforce_t: '8', dragbackforce: '7.3', dragbackforce_t: '8', rightsideforce: '7.0', rightsideforce_t: '8', leftsideforce: '2.5', leftsideforce_t: '8', liftforce: '17.3', liftforce_t: '8',
      },
      { mid: 6, eid: 1, temperature: '26.03', temperature_t: '20', humidity: '88.99', humidity_t: '20', windSpeed: '19.41', windSpeed_t: '15', 
        dragfrontforce: '8.0', dragfrontforce_t: '10', dragbackforce: '7.5', dragbackforce_t: '10', rightsideforce: '7.3', rightsideforce_t: '10', leftsideforce: '1.0', leftsideforce_t: '10', liftforce: '17.9', liftforce_t: '10'
      },
      { mid: 7, eid: 1, temperature: '26.03', temperature_t: '23', humidity: '88.99', humidity_t: '23', windSpeed: '22.22', windSpeed_t: '19', 
        dragfrontforce: '8.0', dragfrontforce_t: '12', dragbackforce: '7.3', dragbackforce_t: '12', rightsideforce: '7.5', rightsideforce_t: '12', leftsideforce: '3.7', leftsideforce_t: '12', liftforce: '32.3', liftforce_t: '12',
      },
      { mid: 8, eid: 1, temperature: '26.03', temperature_t: '24', humidity: '88.99', humidity_t: '24', windSpeed: '21.00', windSpeed_t: '23', 
        dragfrontforce: '8.0', dragfrontforce_t: '14', dragbackforce: '7.3', dragbackforce_t: '14', rightsideforce: '3.5', rightsideforce_t: '14', leftsideforce: '2.0', leftsideforce_t: '14', liftforce: '33.3', liftforce_t: '14',
      },
      { mid: 9, eid: 1, temperature: '26.03', temperature_t: '25', windSpeed: '25.50', windSpeed_t: '27', 
        dragfrontforce: '8.0', dragfrontforce_t: '16', dragbackforce: '7.4', dragbackforce_t: '16', rightsideforce: '3.5', rightsideforce_t: '16', leftsideforce: '2.1', leftsideforce_t: '16', liftforce: '32.0', liftforce_t: '16',
      },
      { mid: 10, eid: 1, windSpeed: '29.04', windSpeed_t: '29', 
        dragfrontforce: '8.0', dragfrontforce_t: '18', dragbackforce: '7.1', dragbackforce_t: '18', rightsideforce: '1.0', rightsideforce_t: '18', leftsideforce: '1.5', leftsideforce_t: '18', liftforce: '29.8', liftforce_t: '18',
      },
      { mid: 11, eid: 1, windSpeed: '37.32', windSpeed_t: '33', 
        dragfrontforce: '8.0', dragfrontforce_t: '20', dragbackforce: '7.0', dragbackforce_t: '20', rightsideforce: '1.0', rightsideforce_t: '20', leftsideforce: '4.0', leftsideforce_t: '20', liftforce: '20.7', liftforce_t: '20'
      }
    ]

    const pressurePoint = [
      { pid: 1, ppoint: '101879.54', ppoint_t: '0', sensor_num: 1, mid: 1 },
      { pid: 2, ppoint: '101879.54', ppoint_t: '0', sensorNum: 2, mid: 2 },
      { pid: 3, ppoint: '101879.54', ppoint_t: '0', sensorNum: 3, mid: 3 },
      { pid: 4, ppoint: '101879.54', ppoint_t: '0', sensorNum: 4, mid: 4 },
      { pid: 5, ppoint: '101879.54', ppoint_t: '0', sensor_num: 1, mid: 1 },
      { pid: 6, ppoint: '101879.54', ppoint_t: '0', sensorNum: 2, mid: 2 },
      { pid: 7, ppoint: '101879.54', ppoint_t: '0', sensorNum: 3, mid: 3 },
      { pid: 8, ppoint: '101879.54', ppoint_t: '0', sensorNum: 4, mid: 4 },
      { pid: 9, ppoint: '101879.54', ppoint_t: '0', sensor_num: 1, mid: 1 },
      { pid: 10, ppoint: '101879.54', ppoint_t: '0', sensorNum: 2, mid: 2 },
      { pid: 11, ppoint: '101879.54', ppoint_t: '0', sensorNum: 3, mid: 3 },
      { pid: 12, ppoint: '101879.54', ppoint_t: '0', sensorNum: 4, mid: 4 }
    ];
    
    return {users, experiments, measurements, pressurePoint};
  }

  genUserId(users: User[]): number {
    return users.length > 0 ? Math.max(...users.map(user => user.rid)) + 1 : 1;
  }

  genExperimentId(experiments: Experiment[]): number {
    return experiments.length > 0 ? Math.max(...experiments.map(experiment => experiment.id)) + 1 : 1;
  }

  /*
  genMeasurementId(measurements: Measurement[]): number {
    return measurements.length > 0 ? Math.max(...measurements.map(measurement => measurement.id)) + 1 : 1;
  }
  */

  genPressurePointId(pressurePoints: PressurePoint[]): number {
    return pressurePoints.length > 0 ? Math.max(...pressurePoints.map(pressurePoint => pressurePoint.id)) + 1 : 1;
  }
}
