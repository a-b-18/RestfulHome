import { Component, OnInit } from '@angular/core';
import { ArduinoService } from '../_services/arduino.service';
import { Observable } from 'rxjs';
import { ArduinoLed } from '../_models/arduinoLed';
import { Router } from '@angular/router';
@Component({
  selector: 'app-tab2',
  templateUrl: 'tab2.page.html',
  styleUrls: ['tab2.page.scss']
})
export class Tab2Page {

  model: any = {}

  constructor(public arduinoService: ArduinoService) { }

  ngOnInit(): void {
  }

  read() {
    this.arduinoService.read(this.model).subscribe(response => {
      console.log(response);
    })
  }

  writeForm() {
    this.arduinoService.write(this.model).subscribe(response => {
      console.log(response);
    })
  }

  write(model: any) {
    console.log(model);
    this.arduinoService.write(model).subscribe(response => {
      console.log(response);
    })
  }

  clearStorage() {
    this.arduinoService.clearStorage();
  }

}
