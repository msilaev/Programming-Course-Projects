from matplotlib import pyplot as plt
import numpy as np


class LinReg:

    """
    Class with methods for calculation of linear regression coeffitients
    and plotting the line.

    Attributes
    ----------
    x : array of features

    y : array of targets

    ax: instance of Axes class to plot the line
    """

    def __init__(self, ax) -> None:
        self.x = []
        self.y = []
        self.ax = ax

    @staticmethod
    def innerproduct(x, y):
        el_prod = [x[i] * y[i] for i in range(len(x))]
        return sum(el_prod)

    def MyLinFit(self):
        N = len(self.x)
        a = (N * self.innerproduct(self.x, self.y) - sum(self.x) * sum(self.y)
             ) / (
            N * self.innerproduct(self.x, self.x) - sum(self.x) * sum(self.x)
        )
        b = (sum(self.y) - a * sum(self.x)) / N
        return a, b

    def LinRegPlt(self):
        a, b = self.MyLinFit()
        xp = np.linspace(-1, 1, 20)
        self.ax.plot(xp, a * xp + b, "r-")
        print(f"My fit: a={b} and b={b}")


class ScatterPLotBuilder(LinReg):
    """
    Class with methods for creating the interactive plot where we can add
    points by the mouse left button and
    then plot linear regression fit by clicking the mouse right button.
    Written on the basis of the matplotlib
    handling events documentaiton
    https://matplotlib.org/stable/users/explain/event_handling.html.

    Attributes
    ----------
    line : array retuned by the ax.plot() function

    ax : instance of the Axes class

    ax: instance of Axes class to plot the line
    """

    def __init__(self, line, ax):
        super().__init__(ax)
        self.line = line[0]
        # This function connects event anf the class instance. When event
        # occurs the dunder method
        # __call__ is triggered which either add a point by redrawing the
        # canvas or draws linear regression
        # fit if the left click.

        self.cid = line[0].figure.canvas.mpl_connect(
            "button_press_event", self)

    def __call__(self, event):
        if event.name == "button_press_event":
            if event.inaxes != self.line.axes:
                return

            if event.button == 3:
                fig.canvas.mpl_disconnect(self.cid)
                self.LinRegPlt()
            else:
                self.x.append(event.xdata)
                self.y.append(event.ydata)

                self.line.set_data(self.x, self.y)
                self.line.set_markersize(10)
            self.line.figure.canvas.draw()


fig, ax = plt.subplots()
ax.set_ylim(-1, 1)
ax.set_xlim(-1, 1)
ax.set_title(
    'Add several points by a mouse click. \n \
    Left click: add points. Right click: finish and plot linear regression.\n'
)
ax.set_xlabel("x (feature)")
ax.set_ylabel("y (target)")

# invisible point object at the origin

line = ax.plot(
    [0], [0], linewidth=0, marker="o", color="b", markersize=0,
    label="line with marker"
)
linebuilder = ScatterPLotBuilder(line, ax)
plt.show()
