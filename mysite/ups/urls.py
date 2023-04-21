from django.urls import path
from ups.views import login_view, register_view, package_status,package_status_change, package_details, update_delivery_address,update_delivery_address_tracknum
from . import views
from django.views.generic.base import TemplateView

urlpatterns = [
    path('', login_view, name='login'),
    path('signup/', register_view, name='signup'),
    path('package_status/', package_status, name='package_status'),
    path('package_status_change/<int:package_id>/', package_status_change, name='package_status_change'),
    path('package/<int:package_id>/', package_details, name='package_detail'),
    path('update_delivery_address_tracknum/<int:package_id>/', update_delivery_address_tracknum, name='update_delivery_address_tracknum'),
    path('update_delivery_address/<int:package_id>/', update_delivery_address, name='update_delivery_address'),
    path('home/', TemplateView.as_view(template_name='registration/home.html'), name='home'),
]