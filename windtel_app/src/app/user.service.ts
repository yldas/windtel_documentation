import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';

import { User } from './user';
import { MessageService } from './message.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable({
  providedIn: 'root'
})
export class UserService {

  public current_user: User;

  //private usersUrl = 'api/users';
  
  
  private baseUrl = 'http://6eb2097a.ngrok.io/WindTel/';
  private usersUrl = this.baseUrl + 'users/';   
  private registerUrl = this.baseUrl + 'register/';
  private deleteUrl = this.baseUrl + 'users';
  private modifyUserUrl = this.baseUrl + 'users/';

  registeredUsers: User[];

  constructor(
    private http: HttpClient,
    private messageService: MessageService) { }

  getRegisteredUsers(): Observable<any> {
    console.log("Got registered users");
    return this.http.get<any>(this.usersUrl);
  }

  setCurrentUser(current_user: User) {
    this.current_user = current_user;
  }

  logout() {
    let u: User;
    this.current_user = u;
  }

  getCurrentUser(): User {
    return this.current_user;
  }

  login(user: User) {
    this.http.post<User>(this.usersUrl, user, httpOptions).pipe(
      catchError(this.handleError<User>('registerUser'))
    );
  }

  getUser(remail: string, rpassword: string, registeredUsers: any[]): User {
    let rusers: User[] = registeredUsers;
    console.log("getUser(): ");
    let nuser: User;

    // UNCOMMENT FOR DB
    registeredUsers.forEach(function (value) {
      if (remail === value.remail && rpassword === value.rpassword) {
        nuser = new User(value.rid, value.rname, value.rlast_name, value.remail, value.rvocation, value.rdepartment, value.rinstitution, value.rcity, value.rpassword, value.rpermissions);
      }
    })
    if (nuser) {
      this.current_user = nuser;
    }
  
    //this.current_user = registeredUsers.find(current_user => current_user.remail === remail && current_user.rpassword == rpassword); 
    if (this.current_user) {
      return this.current_user
    }
  }

  registerUser(user: User): Observable<User> {
    let tempUser = {
      rname: user.rname,
      rlast_name: user.rlast_name,
      remail: user.remail,
      rpassword: user.rpassword,
      rvocation: user.rvocation,
      rdepartment: user.rdepartment,
      rinstitution: user.rinstitution,
      rcity: user.rcity
    };
    return this.http.post<User>(this.registerUrl, tempUser, httpOptions).pipe(
      tap((newUser: User) => console.log(`added user w/id=${newUser.rid}`)),
      catchError(this.handleError<User>('registerUser'))
    );
  }

  deleteUser(user: User | number): Observable<User> {
    const id = typeof user === 'number' ? user: user.rid;
    const url = `${this.deleteUrl}/${id}`;

    return this.http.delete<User>(url, httpOptions).pipe(
      tap(_ => this.log(`deleted user id=${id}`)),
      catchError(this.handleError<User>('deleteUser'))
    );
  }

  changePermissions(user: User): Observable<any> {
    const id = typeof user === 'number' ? user: user.rid;
    const url = `${this.modifyUserUrl}${id}/profile/`;
    let modifiedUser = {
      rname: user.rname,
      rlast_name: user.rlast_name,
      remail: user.remail,
      rpassword: user.rpassword,
      rvocation: user.rvocation,
      rdepartment: user.rdepartment,
      rinstitution: user.rinstitution,
      rcity: user.rcity,
      rpermissions: user.rpermissions
    }
    return this.http.put(url, modifiedUser, httpOptions).pipe(
      tap(_ => this.log(`updated user id=${user.rid}`)),
      catchError(this.handleError<any>('changePermissions'))
    );
  }

  /** Log a UserService message with the MessageService */
  private log(message: string) {
  this.messageService.add(`UserService: ${message}`);
  }

  /**
   * Handle Http operation that failed.
   * Let the app continue.
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
    private handleError<T> (operation = 'operation', result?: T) {
      return (error: any): Observable<T> => {
    
    // TODO: send the error to remote logging infrastructure
    console.log("There was an error!");
    console.error(error); // log to console instead
    
    // TODO: better job of transforming error for user consumption
    this.log(`${operation} failed: ${error.message}`);
    
    // Let the app keep running by returning an empty result.
    return of(result as T);
    };
  }
}
