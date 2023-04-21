from django.contrib import admin
from django.urls import include, path
from django.contrib.auth import views as auth_views
from ups.views import login_view, register_view


urlpatterns = [
    path('', include('ups.urls')),
    path('admin/', admin.site.urls),
    path('', login_view, name='login'),
    path('register/', register_view, name='register')
]