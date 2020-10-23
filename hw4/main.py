import sys
import truss

#check for correct usage
if len(sys.argv) < 3:
    print('Usage:')
    print('  python3 {} [joints file] [beams file] [optional plot output file]'\
    .format(sys.argv[0]))
    sys.exit(0)

joints_file = sys.argv[1]
beams_file = sys.argv[2]

try:
    a = truss.Truss(joints_file,beams_file)
except RuntimeError as e:
    print('ERROR: {}'.format(e))
    sys.exit(2)

#check if plot filename is given and make a plot if given
if len(sys.argv) == 4:
    plot_file = sys.argv[3]
    a.PlotGeometry(plot_file)
    
print(a)
