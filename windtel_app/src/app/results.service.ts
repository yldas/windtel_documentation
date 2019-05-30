import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Data } from './Data'; 
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ResultsService {
  data: Data[];
  //url = 'localhost:4000/results';
  url = 'http://6eb2097a.ngrok.io/WindTel/measurements'
  month = [];
  price = [];
  chart = [];

  constructor(private httpClient: HttpClient) { }

  getResultsFromDatabase(): Observable<any> {
    return this.httpClient.get<any>(this.url);
  }
}
