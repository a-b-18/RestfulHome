import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { map, take } from 'rxjs/operators';
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
  arduinoLed: ArduinoLed = {id: "1", state: "off"};
  
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

  toggle(id: number) {
    // Read led state
    this.read(id).subscribe(response => {
      console.log(response);
      
      // Write opposite state
      this.write({"id": id, "state": (this.arduinoLed.state==="off"?"on":"off")})
      .subscribe(response => {
        console.log(response);
      })
    })
  }

  read(id: number) {
    return this.http.get(this.ledUrl + "?id=" + id).pipe(
      map((response: any) => {
        const led = response;
        if (led) {
          console.log(response);
          this.setcurrentArduino(led);
        }
      })
    )
  }

  setcurrentArduino(led: ArduinoLed) {
    localStorage.setItem('led', JSON.stringify(led));
    this.currentArduinoSource.next(led);
    this.arduinoLed = led;
  }

  clearStorage() {
    localStorage.removeItem('led');
    this.currentArduinoSource.next(null);
  }
}
