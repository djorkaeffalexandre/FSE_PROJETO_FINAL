import React from 'react';

import Switch from '@material-ui/core/Switch';
import Paper from '@material-ui/core/Paper';
import Grid from '@material-ui/core/Grid';

import Server from './Server';

import useStyles from './styles';

const Connected = ({ item }) => {
  const classes = useStyles();

  const onChange = () => {
    Server.setOutput(item);
  };

  return (
    <Grid item xs={12} sm={6}>
      <Paper id={item.mac} className={classes.paper}>
        <p>Cômodo: {item.name}</p>
        <p>Alimentação: {item.type}</p>
        {item.type === 'ENERGY' && (
          <>
            <p>Temperatura: {item.temperature ?? 0}º</p>
            <p>Umidade: {item.humidity ?? 0}º</p>
          </>
        )}
        <p>{item.inputName}</p>
        <div className={item.input ? classes.input : classes.noInput}></div>
        {item.type === 'ENERGY' && (
          <>
            <p>{item.outputName}</p>
            <Switch
              color='primary'
              name='checked'
              checked={item.output ?? false}
              inputProps={{ 'aria-label': 'primary checkbox' }}
              onChange={onChange}
            />
          </>
        )}
      </Paper>
    </Grid>
  );
}

export default Connected;
