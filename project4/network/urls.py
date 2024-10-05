
from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("register", views.register, name="register"),
    path("profile/<int:user_id>", views.profile, name="profile"),

    path("compose", views.new_post, name="compose"),
    path("posts/<str:box>/<int:page>", views.post_box, name="display"),
    path("postsprof/<int:user_id>/<int:page>", views.person_box, name="displayprof"),
    path("follow/<int:user_id>", views.follow, name="follow"),
    path("num_pages/<str:box>", views.num_pages, name="countpages"),
    path("edit/<int:post_id>", views.edit_post, name="editpost"),
    path("like/<int:post_id>", views.like_unlike, name="like")
]
