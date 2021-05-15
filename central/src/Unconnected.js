import React from 'react';

import Button from '@material-ui/core/Button';
import Paper from '@material-ui/core/Paper';
import Grid from '@material-ui/core/Grid';

import useStyles from './styles';

const Unconnected = ({ item, onClick }) => {
  const classes = useStyles();

  return (
    <Grid item xs={12} sm={6}>
      <Paper className={classes.paper}>
        <p>MAC Address: {item.mac}</p>
        <p>Type: {item.type}</p>
        <Button variant='contained' color='primary' onClick={() => onClick?.(item)}>
          Cadastrar
        </Button>
      </Paper>
    </Grid>
  );
}

export default Unconnected;
