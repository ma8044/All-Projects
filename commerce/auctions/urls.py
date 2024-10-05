from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("register", views.register, name="register"),
    path("create", views.create, name="create" ),
    path("listing/<int:listing_id>", views.listing, name="listing"),
    path("close/<int:listing_id>", views.close, name="close"),
    path("watchlist", views.watchlist, name="watchlist"),
    path("addtowatchlist/<int:listing_id>", views.addtowatchlist, name="addtowatchlist"),
    path("wonlistings", views.wonlistings, name="wonlistings"),
    path("categories", views.showcategories, name="categories"),
    path("category/<str:category>", views.category, name="category"),
    path("comment/<int:listing_id>", views.comment, name="comment"),
    path("mylistings", views.mylistings, name="mylistings")
]   
