import React, { useState } from 'react';

import Modal from '@material-ui/core/Modal';
import TextField from '@material-ui/core/TextField';
import Checkbox from '@material-ui/core/Checkbox';
import Button from '@material-ui/core/Button';

import { getModalStyle } from './Utils';
import useStyles from './styles';

const Register = ({ open, submit, item, onClose }) => {
  const classes = useStyles();

  const [data, setData] = useState({
    inputName: '',
    outputName: '',
    name: '',
    alarm: false
  });

  const onSubmit = (event) => {
    event.preventDefault();
    submit({
      ...item,
      ...data
    });
  }

  const onInput = (key, value) => {
    setData({ ...data, [key]: value });
  }

  return (
    <Modal
      open={open === item.mac}
      onClose={onClose}
      aria-labelledby='simple-modal-title'
      aria-describedby='simple-modal-description'
    >
      <div style={getModalStyle()} className={classes.paper2}>
        <h2 id='simple-modal-title'>Definir dados da placa</h2>
        <div id='simple-modal-description'>
          <form noValidate autoComplete='off' onSubmit={onSubmit}>
            <TextField
              id='input'
              label='Nome da Entrada'
              value={data.inputName}
              onInput={e => onInput('inputName', e.target.value)}
            />
            <br /><br />
            {item.type === 'ENERGY' && (
              <>
                <TextField
                  id='output'
                  label='Nome da Saída'
                  value={data.outputName}
                  onInput={e => onInput('outputName', e.target.value)}
                />
                <br /><br />
              </>
            )}
            <TextField
              id='name'
              label='Nome do Cômodo'
              value={data.name}
              onInput={e => onInput('name', e.target.value)}
            />
            <br /><br />
            {item.type === 'ENERGY' && (
              <>
                <p>Entrada dispara alarme?</p>
                <Checkbox
                  checked={data.alarm}
                  onChange={e => onInput('alarm', e.target.checked)}
                  inputProps={{ 'aria-label': 'primary checkbox' }}
                />
                <br /><br />
              </>
            )}
            <Button
              key={1}
              color='primary'
              variant='contained'
              type='submit'
            >
              Cadastrar
           </Button>
          </form>
        </div>
      </div>
    </Modal>
  );
}

export default Register;
