from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("wiki/", views.index, name="index1"),
    path("wiki/"+"<str:fileName>", views.display, name="display"),
    path("search/", views.search, name="search"),
    path("newPage/", views.new, name="new"),
    path("edit/"+"<str:fileName>", views.edit, name="edit"),
    path("random/", views.random, name="random")
]
