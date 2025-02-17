from ._dsp_plot_dspline import Dsp
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, CheckButtons
import numpy as np

class DspPlot:
    def __init__(self, input_file, initial_indexes):

        plt.rcParams["font.family"]="monospace"

        self.vals = self.__read_file(input_file)
        self.N = len(self.vals)
        self.initial_indexes = initial_indexes

        self.fig = plt.figure()
        self.ax = self.fig.add_subplot()
        self.ax_fd = self.ax.twinx()
        self.ax.set_xlim((-1, self.N))
        self.ax.grid(axis="x")

        self.ms = 0.5
        self.dsp_line, = self.ax.plot((np.arange(3*self.N+2)-2)/3,np.zeros(3*self.N+2),marker="o", c="b",ms=self.ms)
        self.func_line, = self.ax.plot(np.arange(self.N),self.vals, marker="o", c="black", ms=self.ms, mec="black", mfc="white") 
        self.fd_line, = self.ax_fd.plot(np.arange(1,self.N-1),np.zeros(self.N-2),marker="o", c="r", ms=self.ms)
        self.added_pt, = self.ax.plot([],[], marker="x", ls="None", mew=1, ms=10, mec="magenta",  zorder=2.5)
        self.idx_text = self.ax.text(0.99, 0.99, "", va='top', ha='right', transform=self.ax.transAxes)

        self.opt_line = self.ax.axvline(x=np.argmin(self.vals),c="orangered")
        self.min_line = self.ax.axvline(x=0,c="steelblue")
        self.nxt_line = self.ax.axvline(x=0,c="magenta")

        plt.subplots_adjust(bottom=0.2)
        self.nxt_btn = Button(self.fig.add_axes([0.8, 0.05, 0.11, 0.075]), "next")
        self.nxt_btn.on_clicked(self.__nxt)
        self.pre_btn = Button(self.fig.add_axes([0.68, 0.05, 0.11, 0.075]), "previous")
        self.pre_btn.on_clicked(self.__prev)
        self.reset_btn = Button(self.fig.add_axes([0.56, 0.05, 0.11, 0.075]), "reset")
        self.reset_btn.on_clicked(self.__init)

        self.lines = [self.added_pt, self.fd_line]
        self.labels = ["sample point", r"$|f_{i-1}-2f_{i}+f_{i+1}|$"]
        self.visiblities = [True, False]
        self.check_box = CheckButtons(self.fig.add_axes([0.1, 0.02, 0.25, 0.12]), self.labels, self.visiblities)
        self.check_box.on_clicked(self.__check)
        self.fd_line.set_visible(False)
        self.ax_fd.axes.yaxis.set_visible(False)

    def __read_file(self, input_file):
        vals = []
        with open(input_file,encoding="utf-8") as f:
            for line in f:
                line = line.split("#")[0].strip()
                if line:
                    vals.append(float(line))
        return vals

    def __init(self, event=None):
        self.__reset_dsp()
        self.__graph_update()

    def start_plot(self):
        self.__init()
        plt.show()

    def __reset_dsp(self):
        self.dsp = Dsp(len(self.vals)) 
        for init_idx in self.initial_indexes:
            self.dsp.add(init_idx, self.vals[init_idx])
        self.dsp.update()

    def __graph_update(self):
        self.dsp_line.set_ydata(self.dsp.get_f())
        self.fd_line.set_ydata(self.dsp.get_fd())
        self.added_pt.set_xdata(self.dsp.get_added_i())
        self.added_pt.set_ydata(self.dsp.get_added_v())

        self.current_idx = self.dsp.get_current()
        self.nxt_idx = self.dsp.get_nxt()
        self.min_idx = self.dsp.get_min()
        self.min_cnt = self.dsp.get_min_cnt()
        self.idx_text.set_text(f"current:{self.current_idx:>4}\nnext:{self.nxt_idx:>4}\nmin:{self.min_idx:>4}\nmin count:{self.min_cnt:>4}")

        self.nxt_line.set_xdata([self.nxt_idx])
        self.min_line.set_xdata([self.min_idx])

        self.ax_fd.relim()
        self.ax_fd.autoscale_view()
        self.fig.canvas.draw()
        self.fig.canvas.flush_events()

    def __nxt(self, event=None):
        nxt_idx = self.dsp.get_nxt()
        self.dsp.add(nxt_idx, self.vals[nxt_idx])
        self.dsp.update()
        self.__graph_update()

    def __prev(self, event=None):
        if len(self.dsp.get_added_i()) == len(self.initial_indexes): 
            return

        current_idx = self.dsp.get_current()
        self.__reset_dsp()
        while (nxt_idx := self.dsp.get_nxt()) != current_idx:
            self.dsp.add(nxt_idx, self.vals[nxt_idx])
            self.dsp.update()
        self.__graph_update()

    def __check(self, label):
        idx = self.labels.index(label)
        self.lines[idx].set_visible(not self.lines[idx].get_visible())
        self.ax_fd.axes.yaxis.set_visible(self.fd_line.get_visible())
        self.fig.canvas.draw()

    def __dsp_1d(self):
        self.standing_idx = (self.N-1)//2
        while (opt_idx := self.__srch_opt()) != self.standing_idx:
            self.standing_idx = opt_idx
        self.__graph_update()

    def __srch_opt(self):
        self.min_idx = self.dsp.get_min()
        self.min_cnt = 1
        while self.min_cnt < 3:
            self.nxt_idx = self.dsp.get_nxt()
            self.dsp.add(self.nxt_idx, self.vals[self.nxt_idx])
            self.dsp.update()

            if self.min_idx == self.dsp.get_min():
                self.min_cnt += 1
            else:
                self.min_idx = self.dsp.get_min()
                self.min_cnt = 1
        return self.min_idx
