import mqtt from 'mqtt';

const MATRICULA = 160026822;

class Server {

  items = {};
  connected = {};

  constructor() {
    this.client = mqtt.connect('mqtt://test.mosquitto.org:8080');
  
    this.client.on('connect', this._onConnect);
    this.client.on('message', this._onMessage);

    const itemsString = localStorage.getItem('items');
    if (itemsString) {
      this.items = JSON.parse(itemsString);
    }

    const connectedString = localStorage.getItem('connected');
    if (connectedString) {
      this.connected = JSON.parse(connectedString);
    }
  }

  _handleChange = () => {
    localStorage.setItem('items', JSON.stringify(this.items));
    localStorage.setItem('connected', JSON.stringify(this.connected));

    this.handleConnection?.(Object.values(this.items));
    this.handleConnected?.(Object.values(this.connected));
  }

  _onConnect = () => {
    this.client.subscribe(`fse2020/${ MATRICULA }/dispositivos/+`, (err) => {
      if (err) {
        console.error('Can\'t subscribe');
      }
    })
    Object.values(this.connected).forEach(item => this.subscribe(item));
  }

  _onMessage = (topic, message) => {
    const data = JSON.parse(new TextDecoder('utf-8').decode(message));
    if (topic.includes('dispositivos')) {
      if (data.type === 'ENERGY' || data.type === 'BATTERY') {
        this.items[data.mac] = data;
      }
    }
    if (this.connected[data.mac]) {
      if (topic.includes('estado')) {
        if (this.connected[data.mac].input !== data.input) {
          this._handleLog(`esp-${ data.mac }`, 'UPDATE-INPUT');
        }
        if (this.connected[data.mac].output !== data.output) {
          this._handleLog(`esp-${ data.mac }`, 'UPDATE-OUTPUT');
        }

        this.connected[data.mac].input = data.input;
        this.connected[data.mac].output = data.output;
      }
      if (topic.includes('temperatura')) {
        this.connected[data.mac].temperature = data.temperature;
      }
      if (topic.includes('umidade')) {
        this.connected[data.mac].humidity = data.humidity;
      }
    }

    this._handleChange();
  }

  delete = (item) => {
    this.unsubscribe(item);

    this.client.publish(
      `fse2020/${ MATRICULA }/dispositivos/${ item.mac }`,
      JSON.stringify({
        type: 'UNREGISTER'
      })
    );

    delete this.connected[item.mac];

    this._handleChange();
    this._handleLog(`esp-${ item.mac }`, 'DELETE');
  }

  register = (item) => {
    this.connected[item.mac] = item;

    this.client.publish(
      `fse2020/${ MATRICULA }/dispositivos/${ item.mac }`,
      JSON.stringify({
        type: 'REGISTER',
        name: item.name
      })
    );

    this.subscribe(item);

    delete this.items[item.mac];

    this._handleChange();
    this._handleLog(`esp-${ item.mac }`, 'REGISTER');
  }

  subscribe = (item) => {
    this.client.subscribe(
      `fse2020/${ MATRICULA }/${ item.name }/estado`,
      (err) => {
        if (err) {
          console.error('Can\'t subscribe');
        }
      }
    );

    if (this.connected[item.mac].type === 'ENERGY') {
      this.client.subscribe(
        `fse2020/${ MATRICULA }/${ item.name }/temperatura`,
        (err) => {
          if (err) {
            console.error('Can\'t subscribe');
          }
        }
      );
  
      this.client.subscribe(
        `fse2020/${ MATRICULA }/${ item.name }/umidade`,
        (err) => {
          if (err) {
            console.error('Can\'t subscribe');
          }
        }
      );
    }
  }

  unsubscribe = (item) => {
    this.client.unsubscribe(
      `fse2020/${ MATRICULA }/${ item.name }/estado`,
      (err) => {
        if (err) {
          console.error('Can\'t unsubscribe');
        }
      }
    );

    if (this.connected[item.mac].type === 'ENERGY') {
      this.client.unsubscribe(
        `fse2020/${ MATRICULA }/${ item.name }/temperatura`,
        (err) => {
          if (err) {
            console.error('Can\'t unsubscribe');
          }
        }
      );
  
      this.client.unsubscribe(
        `fse2020/${ MATRICULA }/${ item.name }/umidade`,
        (err) => {
          if (err) {
            console.error('Can\'t unsubscribe');
          }
        }
      );
    }
  }

  setOutput = (item) => {
    this.connected[item.mac] = item;

    this.client.publish(
      `fse2020/${ MATRICULA }/dispositivos/${ item.mac }`,
      JSON.stringify({ type: 'SET_OUTPUT' })
    );

    this._handleChange();
    this._handleLog(`esp-${ item.mac }`, 'SET_OUTPUT');
  }
  
  _handleLog = (item, action) => {
    const old = JSON.parse(localStorage.getItem('log') ?? '["item", "action"]');
    old.push(item);
    old.push(action);
    localStorage.setItem('log', JSON.stringify(old));
    this.handleLog?.(JSON.stringify(old.join(',')));
  }
  
}

const server = new Server();
export default server;
