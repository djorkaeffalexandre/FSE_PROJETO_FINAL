import { makeStyles } from '@material-ui/core/styles';

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
  },
  input: {
    width: 24,
    height: 24,
    borderRadius: 12,
    backgroundColor: 'green'
  },
  noInput: {
    width: 24,
    height: 24,
    borderRadius: 12,
    backgroundColor: 'red'
  }
}));

export default useStyles;
