import './App.css';
import { makeStyles } from '@material-ui/core/styles';
import Paper from '@material-ui/core/Paper';
import Grid from '@material-ui/core/Grid';
import Switch from '@material-ui/core/Switch';
import Button from '@material-ui/core/Button';
import Modal from '@material-ui/core/Modal';
import TextField from '@material-ui/core/TextField';
import Checkbox from '@material-ui/core/Checkbox';

import React from 'react';

function rand() {
  return Math.round(Math.random() * 20) - 10;
}

function getModalStyle() {
  const top = 50 + rand();
  const left = 50 + rand();

  return {
    top: `${top}%`,
    left: `${left}%`,
    transform: `translate(-${top}%, -${left}%)`,
  };
}

const useStyles = makeStyles((theme) => ({
  root: {
    flexGrow: 1,
  },
  paper: {
    padding: theme.spacing(2),
    textAlign: 'center',
    color: theme.palette.text.secondary,
  },
  paper2: {
    position: 'absolute',
    width: 400,
    backgroundColor: theme.palette.background.paper,
    border: '2px solid #000',
    boxShadow: theme.shadows[5],
    padding: theme.spacing(2, 4, 3),
  }
}));

function App() {
  const classes = useStyles();

  const [state, setState] = React.useState({
    checked: true
  });
  const [open, setOpen] = React.useState(false);
  const [modalStyle] = React.useState(getModalStyle);
  const [checkedAlarm, setCheckedAlarm] = React.useState(true);
  const [input, setInput] = React.useState('');
  const [output, setOutput] = React.useState('');
  const [name, setName] = React.useState('');
  const [selectedEsp, setSelectedEsp] = React.useState({});
  const [espObj, setEspObj] = React.useState([
    { mac: "abcdef", type: "energy" },
    { mac: "ghijkl", type: "energy" },
    { mac: "mnopqr", type: "battery" }
  ]);

  const handleChangeCheckbox = (event) => {
    setCheckedAlarm(event.target.checked);
  };

  function handleSubmit(esp) {

    let obj = espObj.find(o => o.mac === esp.mac);
    let indexObj = espObj.findIndex(o => o.mac === esp.mac);
    let newObj = {
      mac: obj.mac,
      type: obj.type,
      input: input,
      output: output,
      name: name,
      checkedAlarm: checkedAlarm
    }
    let stateEspObj = espObj;
    stateEspObj[indexObj] = newObj;
    setEspObj(stateEspObj);

    handleClose();

  }

  const handleChange = (event) => {
    setState({ ...state, [event.target.name]: event.target.checked });
  };

  const handleOpen = (esp) => {
    setSelectedEsp(esp);
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  return (
    <div className={classes.root}>
      <div style={{ marginLeft: 40, marginTop: 20 }}>
        Alarme:       <Switch
          checked={state.checked}
          onChange={handleChange}
          color="primary"
          name="checked"
          inputProps={{ 'aria-label': 'primary checkbox' }}
        />
      </div>
      {espObj.map((esp) => (
        <Grid container spacing={3}>
          <Grid item xs={12} sm={6}>
            <Paper className={classes.paper}>
              MAC Address: {esp.mac} <br />
              Type: {esp.type} <br /> <br />
              <Button variant="contained" color="primary" onClick={() => handleOpen(esp)}>
                Editar dados
              </Button>
              <Modal
                open={open}
                onClose={handleClose}
                aria-labelledby="simple-modal-title"
                aria-describedby="simple-modal-description"
              >
                <div style={modalStyle} className={classes.paper2}>
                  <h2 id="simple-modal-title">Definir dados da placa</h2>
                  <div id="simple-modal-description">
                    <form noValidate autoComplete="off" onSubmit={(e) => { e.preventDefault(); handleSubmit(selectedEsp) }}>
                      <TextField
                        id="input"
                        label="Nome da Entrada"
                        // style={{ width: 300 }}
                        value={input}
                        onInput={e => setInput(e.target.value)}
                      />
                      <br /><br />
                      <TextField
                        id="output"
                        label="Nome da Saída"
                        // style={{ width: 300 }}
                        value={output}
                        onInput={e => setOutput(e.target.value)}
                      />
                      <br /><br />
                      <TextField
                        id="name"
                        label="Nome do Cômodo"
                        // style={{ width: 300 }}
                        value={name}
                        onInput={e => setName(e.target.value)}
                      />
                      <br /><br />

                        Ativa alarme?       <Checkbox
                        checked={checkedAlarm}
                        onChange={handleChangeCheckbox}
                        inputProps={{ 'aria-label': 'primary checkbox' }}
                      />
                      <br /><br />

                      <Button
                        key={1}
                        color='primary'
                        variant='contained'
                        type="submit"
                      >
                        Enviar
                      </Button>
                    </form>
                  </div>
                </div>
              </Modal>
            </Paper>
          </Grid>
          <Grid item xs={12} sm={6}>
            <Paper id={esp.mac} className={classes.paper}>{esp.input} <br /> {esp.type} <br /> {esp.output} <br /> {esp.name} <br /> Sensor de Presença: {esp.checkedAlarm ? "Ativado" : "Desativado"} </Paper>
          </Grid>
        </Grid>
      ))}

    </div>
  );
}

export default App;