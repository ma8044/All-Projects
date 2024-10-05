import json
from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from django.core.paginator import Paginator

from .models import *


def index(request):
    return render(request, "network/index.html")


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
            return render(request, "network/login.html", {
                "message": "Invalid username and/or password."
            })
    else:
        return render(request, "network/login.html")


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
            return render(request, "network/register.html", {
                "message": "Passwords must match."
            })

        # Attempt to create new user
        try:
            user = User.objects.create_user(username, email, password)
            user.save()
        except IntegrityError:
            return render(request, "network/register.html", {
                "message": "Username already taken."
            })
        login(request, user)
        return HttpResponseRedirect(reverse("index"))
    else:
        return render(request, "network/register.html")


@csrf_exempt
@login_required
def new_post(request):
    if request.method != "POST":
        return JsonResponse({"error": "POST request required."}, status=400)
    
    data = json.loads(request.body)
    content = data.get("content", "")
    post = Post(
        creator = request.user,
        body = content
    )
    post.save()

    return JsonResponse({"message": "Post Saved Successfully!"}, status=201)



def post_box(request, box, page):
    try:
        if box == "all":
            posts = Post.objects.all()
        elif box == "following":
            following = request.user.following.all()
            posts = Post.objects.none()
            for user in following:
                print(user.username)
                print(user.posts.count())
                posts = posts.union(user.posts.all())
        else:
            return JsonResponse({"error": "Invalid Postbox"}, status=400)
        
        posts = posts.order_by("-timestamp").all()
        serialized_posts = [post.serialize() for post in posts]
        posts = Paginator(serialized_posts, 10)


        return JsonResponse(posts.page(page).object_list, safe=False)
    except Exception as e:
        print(f"Error: {e}")
        return JsonResponse({"error": str(e)}, status=500)
    

def profile(request, user_id):
    return render(request, "network/profile.html", {
        "profile" : User.objects.get(id = user_id)
    })


def num_pages(request, box):
    try:
        if box == "all":
            posts = Post.objects.all()
        elif box == "following":
            following = request.user.following.all()
            posts = Post.objects.none()
            for user in following:
                posts = posts.union(user.posts.all())
        elif box.isdigit():
            try:
                profile = User.objects.get(pk = int(box))
                posts = profile.posts.all()
            except User.DoesNotExist:
                return JsonResponse({"error": "User not found"}, status=400)
        else:
            return JsonResponse({"error": "Invalid Postbox"}, status=400)

        serialized_posts = [post.serialize() for post in posts]
        posts = Paginator(serialized_posts, 10)


        return JsonResponse({"num_pages": posts.num_pages}, safe=False)
    except Exception as e:
        print(f"Error: {e}")
        return JsonResponse({"error": str(e)}, status=500)


def person_box(request, user_id, page):
    try:
        print("HAHA!")
        profile = User.objects.get(pk = user_id)
        posts= profile.posts.all()
        posts = posts.order_by("-timestamp").all()
        serialized_posts = [post.serialize() for post in posts]
        posts = Paginator(serialized_posts, 10)

        return JsonResponse(posts.page(page).object_list, safe=False)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=500)

@csrf_exempt
@login_required
def follow(request, user_id):
    try:
        profile = User.objects.get(pk = user_id)
        user = request.user
        print(request.method)
    except User.DoesNotExist:
        return JsonResponse({"error": "User not found"}, status=400)
    
    if request.method == "PUT" and user != profile:
        try:
            data = json.loads(request.body)
            if data.get("follow") == True and profile not in user.following.all():
                user.following.add(profile)
                user.save()
            elif data.get("follow") == False and profile in user.following.all():
                user.following.remove(profile)
                user.save()
            else:
                print("put request could not be completed")
                return JsonResponse({"error": "PUT request could not be completed."}, status=400)
            return HttpResponse(status=204)
        except Exception as e:
            print(e)
            return JsonResponse({"error": str(e)}, status=400)
    else:
        print("PUT request required")
        return JsonResponse({"error": "PUT request required or invalid follow request"}, status=400)

@csrf_exempt
@login_required
def edit_post(request, post_id):
    try:
        post = Post.objects.get(pk = post_id)
        user = request.user
    except Post.DoesNotExist:
        return JsonResponse({"error": "Post not found"}, status=400)
    
    if request.method == "POST" and user == post.creator:
        data = json.loads(request.body)
        content = data.get("content", "")
        post.body = content
        post.edit = True
        post.save()

        return JsonResponse(post.serialize())
    else:
        return JsonResponse({"error": "Edit Unsuccessful. Check request user and request method"}, status=400)

@csrf_exempt
@login_required
def like_unlike(request, post_id):
    
    if request.method != "PUT":
        return JsonResponse({"error": "Invalid Request Method"}, status=400)
    
    try:
        post = Post.objects.get(pk = post_id)
        user = request.user
    except Post.DoesNotExist:
        return JsonResponse({"error": "Post not found"}, status=400)
    
    try:
        data=json.loads(request.body)
        if data.get("like") == True and user not in post.likes.all():
            post.likes.add(user)
            post.save
        elif data.get("like") == False and user in post.likes.all():
            post.likes.remove(user)
            post.save
        else:
            return JsonResponse({"error":"Request could not be completed"}, status = 400)
        return HttpResponse(status=204)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)