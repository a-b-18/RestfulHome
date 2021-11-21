import { Component } from '@angular/core';
import { HTTP } from '@ionic-native/http/ngx';

@Component({
  selector: 'app-tab2',
  templateUrl: 'tab2.page.html',
  styleUrls: ['tab2.page.scss']
})
export class Tab2Page {
  constructor(private http: HTTP) {}

  async getData() {
    try {
      const url = 'http://192.168.1.100/led';
      const params = {};
      const headers = {};

      const response = await this.http.get(url, params, headers);

      console.log(response.status);
      console.log(JSON.parse(response.data)); // JSON data returned by server
      console.log(response.headers);

    } catch (error) {
      console.error(error.status);
      console.error(error.error); // Error message as string
      console.error(error.headers);
    }
  }
}
