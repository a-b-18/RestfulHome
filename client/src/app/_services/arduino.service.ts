import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { map } from 'rxjs/operators';
import { ArduinoLed } from '../_models/arduinoLed';
import { ReplaySubject } from 'rxjs';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class ArduinoService {
  ledUrl = environment.arduinoLedUrl;
  private currentArduinoSource = new ReplaySubject<ArduinoLed>(1);
  currentArduino$ = this.currentArduinoSource.asObservable();

  constructor(private http: HttpClient) { }

  write(model: any) {
    return this.http.put(this.ledUrl + "?id=" + model.id + "&state=" + model.state, model).pipe(
      map((response: ArduinoLed) => {
        const led = response;
        if (led) {
          this.setcurrentArduino(led);
        }
      })
    )
  }

  read(model: any) {
    return this.http.get(this.ledUrl + "?id=" + model.id, model).pipe(
      map((response: any) => {
        const led = response;
        if (led) {
          this.setcurrentArduino(led);
        }
      })
    )
  }

  setcurrentArduino(led: ArduinoLed) {
    localStorage.setItem('led', JSON.stringify(led));
    this.currentArduinoSource.next(led);
  }

  clearStorage() {
    localStorage.removeItem('led');
    this.currentArduinoSource.next(null);
  }
}
