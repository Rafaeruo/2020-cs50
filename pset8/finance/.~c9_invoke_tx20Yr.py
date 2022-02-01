import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.globals.update(usd=usd)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_stocks = db.execute("SELECT symbol, shares FROM shares WHERE user_id = :uid AND shares > :qt", uid=session["user_id"], qt=0)
    total = 0
    for i in user_stocks:
        info = lookup(i['symbol'])
        i['name'] = info['name']
        i['price'] = info['price']
        total += (i['price'] * i['shares'])

    cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session['user_id'])[0]['cash']

    return render_template("index.html", stocks=user_stocks, total=total, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        #ensure the symbol is valid
        if not request.form.get("symbol") or not lookup(request.form.get("symbol")):
            return apology("must input a valid symbol", 403)

        #ensure the number of shares is valid
        elif not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("must input a valid amount of shares")

        #ensure user has enough cash
        cash = float(db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]['cash'])
        if cash < ( lookup(request.form.get("symbol"))['price'] * float(request.form.get("shares")) ):
            return apology("you don't have enough cash to buy this amount of stocks", 403)

        #subtract purchase amount from user's cash
        newcash = cash - float(( lookup(request.form.get("symbol"))['price'] * float(request.form.get("shares")) ))
        db.execute("UPDATE users SET cash = :newcash where id = :uid;", newcash=newcash, uid=session["user_id"])

        #record transaction in database
        now = str(datetime.now())[0:-7]
        db.execute("INSERT INTO history (user_id, operation, symbol, price, shares, datetime) VALUES (:uid, :operation, :symbol, :price, :shares, :datetime)", uid=session["user_id"], operation="b", symbol=request.form.get("symbol").upper(), price=lookup(request.form.get("symbol"))['price'], shares=request.form.get("shares"), datetime=now)

        #update the amount of shares the user has for the company
        user_symbol_instances = db.execute("SELECT * FROM shares WHERE user_id = :uid AND symbol = :symbol", uid=session["user_id"], symbol = request.form.get("symbol").upper())

        #check if user already has already had shares for the company
        if len(user_symbol_instances) == 0:
            db.execute("INSERT INTO shares (symbol, user_id, shares) VALUES (:symbol, :user_id, :shares)", symbol=request.form.get("symbol").upper(), user_id=session["user_id"], shares=request.form.get("shares"))
        else:
            db.execute("UPDATE shares SET shares = shares + :shares WHERE user_id=:uid AND symbol = :symbol", uid=session["user_id"], symbol=request.form.get("symbol").upper(), shares=request.form.get("shares"))


        return render_template("buy.html")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_history = db.execute("SELECT * FROM history WHERE user_id = :uid ORDER BY datetime DESC", uid=session["user_id"])

    return render_template("history.html", history=user_history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        #ensure symbol was submitted
        if not request.form.get("symbol") and not request.form.get("shares"):
            return apology("must provide input", 403)
        #check which input was posted
        elif not request.form.get("shares"):#if it was symbol
            info = lookup(request.form.get("symbol"))
            #ensure lookup found info
            if not info:
                return apology("couldn't retrieve info for that company", 403)

            #organize lookup info
            name = info['name']
            symbol = info['symbol']
            price = info['price']

            #render page with the info
            return render_template("quoted.html", name=name, symbol=symbol, price=usd(price))

        else:#if it was shares
            if int(request.form.get("shares")) < 1:
                return apology("must input a valid amount of shares")

            #ensure user has enough cash
            cash = float(db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]['cash'])
            if cash < ( lookup(request.form.get("symbol2"))['price'] * float(request.form.get("shares")) ):
                return apology("you don't have enough cash to buy this amount of stocks", 403)

            #subtract purchase amount from user's cash
            newcash = cash - float(( lookup(request.form.get("symbol2"))['price'] * float(request.form.get("shares")) ))
            db.execute("UPDATE users SET cash = :newcash where id = :uid;", newcash=newcash, uid=session["user_id"])

            #record transaction in database
            now = str(datetime.now())[0:-7]
            db.execute("INSERT INTO history (user_id, operation, symbol, price, shares, datetime) VALUES (:uid, :operation, :symbol, :price, :shares, :datetime)", uid=session["user_id"], operation="b", symbol=request.form.get("symbol2").upper(), price=lookup(request.form.get("symbol2"))['price'], shares=request.form.get("shares"), datetime=now)

            #update the amount of shares the user has for the company
            user_symbol_instances = db.execute("SELECT * FROM shares WHERE user_id = :uid AND symbol = :symbol", uid=session["user_id"], symbol = request.form.get("symbol2").upper())

            #check if user already has already had shares for the company
            if len(user_symbol_instances) == 0:
                db.execute("INSERT INTO shares (symbol, user_id, shares) VALUES (:symbol, :user_id, :shares)", symbol=request.form.get("symbol2").upper(), user_id=session["user_id"], shares=request.form.get("shares"))
            else:
                db.execute("UPDATE shares SET shares = shares + :shares WHERE user_id=:uid AND symbol = :symbol", uid=session["user_id"], symbol=request.form.get("symbol2").upper(), shares=request.form.get("shares"))


            info = lookup(request.form.get("symbol2"))
            #organize lookup info
            name = info['name']
            symbol = info['symbol']
            price = info['price']

            #render page with the info
            return render_template("quoted.html", name=name, symbol=symbol, price=usd(price))



    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        #ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        #ensure password and confirmation were submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password and password confirmation", 403)

        #ensure password and confirmation are the same
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and password confirmation must be the same")

        #ensure the username isn't already taken
        row = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(row) > 0:
            return apology("username must not have been already taken.", 403)

        #register user to the database
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :password)", username=request.form.get("username"), password=generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))

        #redirect user to /
        return redirect("/")


    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        #ensure the symbol was posted correctly
        if not request.form.get("symbol"):
            return apology("must select a company symbol")
        #ensure the amount of shares is higher than 0
        elif int(request.form.get("shares")) <= 0:
            return apology("must select a number of shares higher than 0")
        #ensure the user has enough shares to sell
        qt = db.execute("SELECT shares FROM shares WHERE user_id = :uid AND symbol = :symbol", uid=session["user_id"], symbol=request.form.get("symbol").upper())
        if not qt:
            return apology("must have shares for the selected company")
        elif qt[0]['shares'] < int(request.form.get("shares")):
            return apology("must have enough shares to sell")

        #add purchase amount from user's cash
        newcash = float(( lookup(request.form.get("symbol"))['price'] * float(request.form.get("shares")) ))
        db.execute("UPDATE users SET cash = cash + :newcash where id = :uid;", newcash=newcash, uid=session["user_id"])

        #record transaction to database
        now = str(datetime.now())[0:-7]
        db.execute("INSERT INTO history (user_id, operation, symbol, price, shares, datetime) VALUES (:uid, :operation, :symbol, :price, :shares, :datetime)", uid=session["user_id"], operation="s", symbol=request.form.get("symbol").upper(), price=lookup(request.form.get("symbol"))['price'], shares=request.form.get("shares"), datetime=now)

        #update amount of shares the user has for the company
        db.execute("UPDATE shares SET shares = shares - :shares WHERE user_id=:uid AND symbol = :symbol", uid=session["user_id"], symbol=request.form.get("symbol").upper(), shares=request.form.get("shares"))


        user_stocks = db.execute("SELECT symbol FROM shares WHERE user_id = :uid", uid=session["user_id"])
        return render_template("sell.html", stocks=user_stocks)
    else:
        user_stocks = db.execute("SELECT symbol, shares FROM shares WHERE user_id = :uid", uid=session["user_id"])
        return render_template("sell.html", stocks=user_stocks)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
