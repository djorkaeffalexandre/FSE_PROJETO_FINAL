import React, { useEffect } from 'react';
import './App.css';

import Grid from '@material-ui/core/Grid';
import Switch from '@material-ui/core/Switch';

import { CSVLink } from 'react-csv';

import Server from './Server';
import Unconnected from './Unconnected';
import Connected from './Connected';

import useStyles from './styles';
import Register from './Register';

const audio = new Audio('https://www.soundjay.com/button/sounds/beep-01a.mp3');

const App = () => {
  const classes = useStyles();

  const [open, setOpen] = React.useState('');
  const [play, setPlay] = React.useState(false);
  const [alarm, setAlarm] = React.useState(false);
  const [connected, setConnected] = React.useState([]);
  const [unconnected, setUnconnected] = React.useState([]);
  const [log, setLog] = React.useState('');

  useEffect(() => {
    Server.handleConnection = setUnconnected;
    Server.handleConnected = setConnected;
    Server.handleLog = setLog;
    Server._handleChange();
  }, []);

  useEffect(() => {
    Server._handleLog('alarm', alarm ? 'ON' : 'OFF');
  }, [alarm]);

  useEffect(() => {
    if (play && alarm) {
      audio.play();
    } else {
      audio.pause();
      audio.load();
    }
  }, [play, alarm]);

  useEffect(() => {
    const shouldPlay = connected
      .filter(item => item.input && item.alarm)
      .length > 0;
    setPlay(shouldPlay);
  }, [connected]);

  const onSubmit = (item) => {
    Server.register(item);
    setOpen('');
  };

  return (
    <div className={classes.root}>
      <div style={{ marginLeft: 40, marginTop: 20 }}>
        <CSVLink data={log}>Download LOG (CSV)</CSVLink>
        <p>Alarme:</p>
        <Switch
          checked={alarm}
          onChange={e => setAlarm(e.target.checked)}
          color='primary'
          name='checked'
          inputProps={{ 'aria-label': 'primary checkbox' }}
        />
      </div>
      <Grid container spacing={3}>
        {unconnected.length === 0 && (
          <Grid item xs={12} sm={6} />
        )}
        {unconnected.map(item => (
          <>
            <Unconnected
              item={item}
              onClick={() => setOpen(item.mac)}
            />
            <Register
              item={item}
              open={open}
              onClose={() => setOpen('')}
              submit={onSubmit}
            />
          </>
        ))}
        {connected.map(item => (
          <Connected item={item} />
        ))}
      </Grid>
    </div>
  );
}

export default App;