from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.decorators import login_required
from .models import *

from .models import User

categories = ["Fashion", "Toys", "Electronics", "Home", "Miscellaneous"]


def index(request):
    return render(request, "auctions/index.html", {
        "title" : "Active Listings",
        "listings" : Listings.objects.filter(active=True)
    })


def login_view(request):
    if request.method == "POST":

        # Attempt to sign user in
        username = request.POST["username"]
        password = request.POST["password"]
        user = authenticate(request, username=username, password=password)

        # Check if authentication successful
        if user is not None:
            login(request, user)
            return HttpResponseRedirect(reverse("index"))
        else:
            return render(request, "auctions/login.html", {
                "message": "Invalid username and/or password."
            })
    else:
        return render(request, "auctions/login.html")


def logout_view(request):
    logout(request)
    return HttpResponseRedirect(reverse("index"))


def register(request):
    if request.method == "POST":
        username = request.POST["username"]
        email = request.POST["email"]

        # Ensure password matches confirmation
        password = request.POST["password"]
        confirmation = request.POST["confirmation"]
        if password != confirmation:
            return render(request, "auctions/register.html", {
                "message": "Passwords must match."
            })

        # Attempt to create new user
        try:
            user = User.objects.create_user(username, email, password)
            user.save()
        except IntegrityError:
            return render(request, "auctions/register.html", {
                "message": "Username already taken."
            })
        login(request, user)
        return HttpResponseRedirect(reverse("index"))
    else:
        return render(request, "auctions/register.html")

@login_required
def create(request):
    if request.method == "POST":
        title = request.POST['title']
        desc = request.POST['description']
        url = request.POST['imgUrl']
        sBid = int(request.POST['bid'])
        current = request.user
        category = request.POST['category']
        listing = Listings(title=title, description=desc, creator=current, img=url, startBid=sBid, category=category)
        bid = Bids(creator=current, bid=sBid, listing=listing)
        listing.save()
        bid.save()
        return HttpResponseRedirect(reverse('index'))

    if request.method == "GET":
        return render(request, "auctions/create.html", {
            "categories": categories
        })

def listing(request, listing_id):
    myListing = Listings.objects.get(id=listing_id)
    if request.method == "POST":
        bid = int(request.POST["bid"])
        sBid = Bids.objects.get(listing=myListing)
        if bid > sBid.bid:
            sBid.creator = request.user
            sBid.bid = bid
            sBid.save()
            return HttpResponseRedirect(reverse("index"))
        else:
            return render(request, "auctions/listing.html", {
                "listing": myListing,
                "message": "Please bid higher than the current bid."
            })

    if request.method == "GET":
        return render(request, "auctions/listing.html", {
            "listing": myListing
        })

@login_required
def close(request, listing_id):
    myListing = Listings.objects.get(id=listing_id)
    if request.method == "POST":
        myListing.active = False
        bidCreator = Bids.objects.get(listing=myListing).creator
        if bidCreator != myListing.creator:
            myListing.winner = bidCreator
        myListing.save()
        return HttpResponseRedirect(reverse("listing", args=(listing_id,)))
    
@login_required
def watchlist(request):
    if request.method == "GET":
        return render(request, "auctions/index.html", {
            "title": "Watchlist",
            "listings": request.user.watchlist.all()
        })
    
@login_required
def addtowatchlist(request, listing_id):
    if request.method == "POST":
        request.user.watchlist.add(Listings.objects.get(id=listing_id))
        return HttpResponseRedirect(reverse("listing", args=(listing_id,)))
    
@login_required
def wonlistings(request):
    return render(request, "auctions/index.html", {
        "title": "Won Listings",
        "listings": request.user.won.all()
    })

def showcategories(request):
    return render(request, "auctions/categories.html", {
        "categories": categories
    })

def category(request, category):
    return render(request, "auctions/index.html", {
        "title": category,
        "listings": Listings.objects.filter(category=category, active=True)
    })

def comment(request, listing_id):
    if request.method == "POST":
        content = request.POST["comment"]
        creator = request.user
        listing = Listings.objects.get(id=listing_id)
        comment = Comments(creator=creator, comment=content, listing=listing)
        comment.save()
        return HttpResponseRedirect(reverse("listing", args=(listing_id,)))
    
@login_required
def mylistings(request):
    return render(request, "auctions/index.html", {
        "title": "My Listings",
        "listings": Listings.objects.filter(creator=request.user)
    })