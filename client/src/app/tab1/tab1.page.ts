import { Component, OnInit } from '@angular/core';
import { ArduinoService } from '../_services/arduino.service';
import { Observable } from 'rxjs';
import { ArduinoLed } from '../_models/arduinoLed';
import { Router } from '@angular/router';
@Component({
  selector: 'app-tab1',
  templateUrl: 'tab1.page.html',
  styleUrls: ['tab1.page.scss']
})
export class Tab1Page {

  model: any = {}

  constructor(public arduinoService: ArduinoService) { }

  ngOnInit(): void {
  }

  read() {
    this.arduinoService.read(this.model).subscribe(response => {
      console.log(response);
    })
  }

  write() {
    this.arduinoService.write(this.model).subscribe(response => {
      console.log(response);
    })
  }

  clearStorage() {
    this.arduinoService.clearStorage();
  }

}
