from . import app
from flask import Flask, render_template, request, url_for
from .game_of_life import GameOfLife
from werkzeug.utils import redirect

from .project.forms import MessageForm

@app.route('/', methods=['GET', 'POST'])
def index():
    height = 25
    width = 25

    form = MessageForm()
    #print(form.validate_on_submit())
    if form.validate_on_submit():
        height = form.height.data
        width = form.width.data
        #print('\n Data received. Redirection ..')

        GameOfLife(height, width)
        #print(f"param {height}, {width}")

        return redirect(url_for('creation'))

    return render_template("index.html", form=form, width=width, heigth=height)

@app.route('/creation', methods=['GET', 'POST'])
def creation():
    #print("creation")

    return render_template("creation.html")

@app.route('/live_box')
def live_box():
    game = GameOfLife()
    if game.counter > 0:
        game.form_new_generation_box()
    game.counter = game.counter + 1
    return render_template("live_box.html", game=game)

@app.route('/live_periodic')
def live_periodic():
    game = GameOfLife()
    if game.counter > 0:
        game.form_new_generation_periodic()
    game.counter = game.counter + 1
    return render_template("live_periodic.html", game=game)

