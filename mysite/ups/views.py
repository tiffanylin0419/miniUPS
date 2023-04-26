from django.http import HttpResponse
from django.contrib.auth import authenticate, login
from django.shortcuts import render, redirect
from django.contrib.auth.forms import UserCreationForm
from ups.forms import CustomUserCreationForm
from django.contrib import messages
from ups.models import Package
from django.contrib.auth.decorators import login_required
from django.core.exceptions import ValidationError

# 注册



def login_view(request):
    if request.method == 'POST':
        # 处理用户提交的登录表单
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password=password)
        if user is not None:
            # 如果用户存在则登录用户
            login(request, user)
            return redirect('home')  # 重定向到主页或其他页面
        else:
            # 如果用户不存在则显示错误消息
            error_message = 'Invalid username or password'
    else:
        error_message = ''
    # 渲染登录页面模板
    return render(request, 'registration/login.html', {'error_message': error_message})

def register_view(request):
    if request.method == 'POST':
        form = CustomUserCreationForm(request.POST)
        if form.is_valid():
            user = form.save()
            user.email = form.cleaned_data.get('email')
            user.save()
            # Log the user in
            login(request, user)
            # Redirect to a success page
            return redirect('login')
    else:
        form = CustomUserCreationForm()
    return render(request, 'registration/signup.html', {'form': form})

def tracking_package(request):
    if request.method == 'POST':
        form = CustomUserCreationForm(request.POST)
        if form.is_valid():
            user = form.save()
            user.email = form.cleaned_data.get('email')
            user.save()
            # Log the user in
            login(request, user)
            # Redirect to a success page
            return redirect('login')
    else:
        form = CustomUserCreationForm()
    return render(request, 'registration/signup.html', {'form': form})

def package_status(request):
    if request.method == 'POST':
        tracking_number = request.POST.get('tracking_number')
        try:
            package = Package.objects.filter(package_id=tracking_number).first()
            return render(request, 'registration/package_status.html', {'package': package})
        except Package.DoesNotExist:
            messages.error(request, 'Invalid tracking number.')
    return render(request, 'registration/package_status.html')

def package_status_change(request,package_id):
    package = Package.objects.get(package_id=package_id)
    return render(request, 'registration/package_status_change.html', {'package': package})

# def home(request):
#     context = {}
#     if request.user.is_authenticated:
#         context['username'] = request.user.username
#         context['packages'] = request.user.package_set.all()
#     return render(request, 'registration/home.html', context)

# def user_packages(request):
#     user = request.user
#     packages = Package.objects.filter(user=user)
#     return render(request, 'registration/user_packages.html', {'packages': packages})

@login_required
def my_packages(request):
    username = request.user.username
    packages = Package.objects.filter(amazon_user_name=username)
    return render(request, 'registration/home.html', {'packages': packages})

def package_details(request, package_id):
    package = Package.objects.get(package_id=package_id, amazon_user_name=request.user.username)
    return render(request, 'registration/package_details.html', {'package': package})


def update_delivery_address(request, package_id):
    package = Package.objects.get(package_id=package_id, amazon_user_name=request.user.username)
    if package.package_status != 'delivered' and package.package_status != 'delivering':
        if request.method == 'POST':
            addr_x = request.POST.get('addr_x', '').strip()
            addr_y = request.POST.get('addr_y', '').strip()
            
            if not addr_x or not addr_y:
                messages.error(request, 'Please enter values for both addr_x and addr_y.')
                return render(request, 'registration/update_delivery_address.html', {'package': package})
            
            package.addr_x = addr_x
            package.addr_y = addr_y
            package.save()
            
            return redirect(package_details, package_id=package.package_id)
        else:
            return render(request, 'registration/update_delivery_address.html', {'package': package})
    else:
        messages.error(request, 'This package has already been delivered.')
        return render(request, 'registration/error.html', {'message': 'This package has already been delivered.'})
    

def update_delivery_address_tracknum(request, package_id):
    package = Package.objects.get(package_id=package_id)
    if package.package_status != 'delivered' and package.package_status != 'delivering':
        if request.method == 'POST':
            addr_x = request.POST.get('addr_x', '').strip()
            addr_y = request.POST.get('addr_y', '').strip()
            
            if not addr_x or not addr_y:
                messages.error(request, 'Please enter values for both addr_x and addr_y.')
                return render(request, 'registration/update_delivery_address_tracknum.html', {'package': package})
            
            package.addr_x = addr_x
            package.addr_y = addr_y
            package.save()
            
            return redirect(package_status_change, package_id=package.package_id)
        else:
            return render(request, 'registration/update_delivery_address_tracknum.html', {'package': package})
    else:
        messages.error(request, 'This package has already been delivered.')
        return redirect(package_status_change, package_id=package.package_id)