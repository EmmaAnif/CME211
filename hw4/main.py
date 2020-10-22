import sys
import truss

if len(sys.argv) < 3:
    print('Usage:')
    print('  python3 {} [joints file] [beams file] [optional plot output file]'\
    .format(sys.argv[0]))
    sys.exit(0)

joints_file = sys.argv[1]
beams_file = sys.argv[2]

if len(sys.argv) == 4:
    plot_file = sys.argv[3]
else:
    plot_file = "notGiven"

try:
    a = truss.Truss(joints_file,beams_file,plot_file)
except RuntimeError as e:
    print('ERROR: {}'.format(e))
    sys.exit(2)

if len(sys.argv) == 4:
    a.PlotGeometry()
print(a)
