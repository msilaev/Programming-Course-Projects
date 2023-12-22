from flask_wtf import FlaskForm
from wtforms import StringField, IntegerField, SubmitField
from wtforms.validators import NumberRange, InputRequired, DataRequired

class MessageForm(FlaskForm) :
    #height = StringField('height:', validators = [NumberRange(1, 100)])
    #width = StringField('width:', validators = [NumberRange(1, 100)])

    width = IntegerField('Enter width', validators=[
        NumberRange(min=5, max=100, message="width should be from 5 to 100"),
        InputRequired('you did not enter'),
        DataRequired("enter positive integer")], default=15)

    height = IntegerField('Enter height', validators=[
        NumberRange(min=5, max=100, message="width should be from 5 to 100"),
        InputRequired('you did not enter'),
        DataRequired("enter positive integer")], default=10)

    submit = SubmitField('Submit')
