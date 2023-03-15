import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from time import gmtime, strftime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Fetch rows from transactions
    rows = db.execute("SELECT symbol, company, SUM(shares) as shares FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    # SELECT symbol, company, SUM(shares) as shares FROM transactions WHERE user_id = 1 GROUP BY symbol;

    # Quote price for each symbol
    for row in rows:
        quote = lookup(row["symbol"])
        row["price"] = quote["price"]

    # Calculate total value of the stock
    # Calaulate the total portfolio value
    portfolio_total = 0
    for row in rows:
        row["total"] = row["price"] * row["shares"]
        portfolio_total += row["total"]

    # Fetch the cash balance of the user
    cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])


    grand_total = cash_balance[0]["cash"] + portfolio_total

    return render_template("index.html", rows=rows, grand_total=grand_total, cash=cash_balance[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure symbol is submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 403)

        quote = lookup(request.form.get("symbol"))

        # Ensure symbol is valid
        if not quote:
            return apology("invalid symbol", 400)

        shares = request.form.get("shares")

        # Ensure shares is submitted
        if not shares:
            return apology("missing shares", 403)

        # Ensure shares is valid
        if not shares.isdigit():
            return apology("invalid shares", 400)

        if int(shares) <= 0:
            return apology("invalid shares", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Ensure there is enough money
        if cash[0]["cash"] < quote["price"] * float(shares):
            return apology("cannot afford", 400)

        # SQL command to create a new table
        # CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, company TEXT NOT NULL, symbol TEXT NOT NULL, price REAL NOT NULL, shares INTEGER NOT NULL, timestamp TEXT NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));
        # CREATE INDEX symbol ON transactions (symbol);
        # CREATE UNIQUE INDEX id on transactions (id);
        # CREATE INDEX user_id on transactions (user_id);

        # Insert new transaction
        db.execute("INSERT INTO transactions (user_id, company, symbol, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], quote["name"], quote["symbol"], quote["price"], int(shares), strftime("%Y-%m-%d %H:%M:%S", gmtime()))

        # Update cash
        new_cash = cash[0]["cash"] - quote["price"] * float(shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT symbol, shares, price, timestamp FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        # Ensure symbol is submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        quote = lookup(request.form.get("symbol"))

        # Ensure symbol is valid
        if not quote:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", quote=quote)


    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username is submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was submitted
        elif not request.form.get("confirmation"):
            return apology("must input password twice", 400)

        # Ensure two password input match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("wrong password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username is unique
        if len(rows) != 0:
            return apology("username exists", 400)

        # Insert the new register
        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", request.form.get("username"), generate_password_hash(request.form.get("confirmation"), method='pbkdf2:sha256', salt_length=8))

        return render_template("login.html")
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Fetch the symbol user owned
    valid_symbol_dict = db.execute("SELECT DISTINCT symbol FROM transactions WHERE user_id = ?", session["user_id"])


    if request.method == "POST":
        # Ensure symbol is submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 403)

        # Create a list to store the symbol
        valid_symbol_list = []

        for valid_symbol in valid_symbol_dict:
            valid_symbol_list.append(valid_symbol["symbol"])

        # Ensure symbol is in portfolio
        if request.form.get("symbol") not in valid_symbol_list:
            return apology("invalid symbol", 400)

        shares = request.form.get("shares")

        # Ensure shares is submitted
        if not shares:
            return apology("missing shares", 403)

        # Ensure shares is valid
        if not shares.isdigit():
            return apology("invalid shares", 400)

        # Cast shares to integer
        shares = int(shares)
        if shares <= 0:
            return apology("invalid shares", 400)

        shares_hold = db.execute("SELECT SUM(shares) as shares FROM transactions WHERE symbol = ? GROUP BY symbol", request.form.get("symbol"))
        if shares > shares_hold[0]["shares"]:
            return apology("invalid shares", 400)

        # Quote current price
        quote = lookup(request.form.get("symbol"))

        # Calculate the value earned by selling the stock
        inflow = quote["price"] * float(shares)

        # Render shares to negative integer
        shares *= -1

        # Update user's cash and transactions table
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", inflow, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, company, symbol, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], quote["name"], quote["symbol"], quote["price"], shares, strftime("%Y-%m-%d %H:%M:%S", gmtime()))

        return redirect("/")

    else:
        return render_template("sell.html", symbols=valid_symbol_dict)

