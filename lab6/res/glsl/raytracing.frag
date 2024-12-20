#version 450 core
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform float refractionIndex;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Hit {
    bool happened;
    vec3 point;
    vec3 normal;
    float t;
    bool isTransparent;
    vec3 color;
};

const vec3 lightPos = vec3(5.0, 5.0, 5.0);
const float ambient = 0.1;
const float AIR_IOR = 1.0;

// Закон Снелля
vec3 snellRefract(vec3 I, vec3 N, float n1, float n2) {
    float eta = n1 / n2;
    float cosI = -dot(N, I);
    float sinT2 = eta * eta * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        // Полное внутреннее отражение
        return reflect(I, N);
    }
    
    float cosT = sqrt(1.0 - sinT2);
    return eta * I + (eta * cosI - cosT) * N;
}

// Формула Френеля
float fresnel(vec3 I, vec3 N, float n1, float n2) {
    float cosI = abs(dot(I, N));
    float sinT2 = (n1 * n1 / (n2 * n2)) * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        return 1.0; // Полное внутреннее отражение
    }
    
    float cosT = sqrt(1.0 - sinT2);
    float Rs = ((n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT));
    float Rp = ((n1 * cosT - n2 * cosI) / (n1 * cosT + n2 * cosI));
    
    return (Rs * Rs + Rp * Rp) / 2.0;
}

Hit intersectSphere(Ray ray, vec3 center, float radius, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 oc = ray.origin - center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.001) {
            hit.happened = true;
            hit.t = t;
            hit.point = ray.origin + t * ray.direction;
            hit.normal = normalize(hit.point - center);
            hit.color = color;
            hit.isTransparent = isTransparent;
        }
    }
    
    return hit;
}

Hit intersectPlane(Ray ray, vec3 normal, float height, vec3 color) {
    Hit hit;
    hit.happened = false;
    
    float denom = dot(normal, ray.direction);
    if (abs(denom) > 0.001) {
        float t = -(dot(ray.origin, normal) + height) / denom;
        if (t > 0.001) {
            hit.happened = true;
            hit.t = t;
            hit.point = ray.origin + t * ray.direction;
            hit.normal = normal;
            hit.color = color;
            hit.isTransparent = false;
            
            // Шахматный узор для пола
            float scale = 1.0;
            bool isWhite = (mod(floor(hit.point.x * scale) + floor(hit.point.z * scale), 2.0) == 0.0);
            hit.color = isWhite ? vec3(0.8) : vec3(0.3);
        }
    }
    
    return hit;
}

Hit intersectCylinder(Ray ray, vec3 base, vec3 axis, float radius, float height, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 rc = ray.origin - base;
    vec3 n = normalize(axis);
    vec3 d = ray.direction;
    
    float a = dot(d, d) - pow(dot(d, n), 2);
    float b = 2.0 * (dot(rc, d) - dot(d, n) * dot(rc, n));
    float c = dot(rc, rc) - pow(dot(rc, n), 2) - radius * radius;
    
    float discriminant = b * b - 4.0 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.001) {
            vec3 p = ray.origin + t * ray.direction - base;
            float h = dot(p, n);
            
            if (h >= 0.0 && h <= height) {
                hit.happened = true;
                hit.t = t;
                hit.point = ray.origin + t * ray.direction;
                hit.normal = normalize(hit.point - (base + h * n));
                hit.color = color;
                hit.isTransparent = isTransparent;
            }
        }
    }
    
    return hit;
}

Hit intersectCone(Ray ray, vec3 apex, vec3 axis, float angle, float height, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 co = ray.origin - apex;
    vec3 n = normalize(axis);
    float cosTheta = cos(angle);
    float sinTheta2 = sin(angle) * sin(angle);
    
    float a = dot(ray.direction, n) * dot(ray.direction, n) - cosTheta * cosTheta;
    float b = 2.0 * (dot(ray.direction, n) * dot(co, n) - dot(ray.direction, co) * cosTheta * cosTheta);
    float c = dot(co, n) * dot(co, n) - dot(co, co) * cosTheta * cosTheta;
    
    float discriminant = b * b - 4.0 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.001) {
            vec3 p = ray.origin + t * ray.direction - apex;
            float h = dot(p, n);
            
            if (h >= 0.0 && h <= height) {
                hit.happened = true;
                hit.t = t;
                hit.point = ray.origin + t * ray.direction;
                vec3 ph = apex + h * n;
                hit.normal = normalize(hit.point - ph);
                hit.color = color;
                hit.isTransparent = isTransparent;
            }
        }
    }
    
    return hit;
}

Hit intersectScene(Ray ray) {
    Hit hit;
    hit.happened = false;
    hit.t = 1e20;
    
    // Прозрачная сфера
    Hit sphere1 = intersectSphere(ray, vec3(0.0, 0.0, -2.0), 0.5, vec3(0.8, 0.8, 1.0), true);
    if (sphere1.happened && sphere1.t < hit.t) hit = sphere1;
    
    // Непрозрачная сфера
    Hit sphere2 = intersectSphere(ray, vec3(1.0, 0.0, -2.5), 0.5, vec3(1.0, 0.2, 0.2), false);
    if (sphere2.happened && sphere2.t < hit.t) hit = sphere2;
    
    // Цилиндр
    Hit cylinder = intersectCylinder(ray, vec3(-1.5, -1.0, -2.5), vec3(0.0, 1.0, 0.0), 0.3, 1.0, vec3(0.2, 0.8, 0.2), false);
    if (cylinder.happened && cylinder.t < hit.t) hit = cylinder;
    
    // Конус
    Hit cone = intersectCone(ray, vec3(1.5, -1.0, -2.5), vec3(0.0, 1.0, 0.0), 0.5, 1.0, vec3(0.8, 0.4, 0.0), false);
    if (cone.happened && cone.t < hit.t) hit = cone;
    
    // Пол
    Hit plane = intersectPlane(ray, vec3(0.0, 1.0, 0.0), 1.0, vec3(0.5));
    if (plane.happened && plane.t < hit.t) hit = plane;
    
    return hit;
}

vec3 shade(Hit hit, Ray ray) {
    if (!hit.happened) return vec3(0.1); // Фоновый цвет
    
    vec3 finalColor = vec3(0.0);
    vec3 throughput = vec3(1.0);
    
    for (int bounce = 0; bounce < 4; bounce++) {
        if (!hit.isTransparent) {
            // Для непрозрачных объектов
            vec3 lightDir = normalize(lightPos - hit.point);
            float diff = max(dot(hit.normal, lightDir), 0.0);
            
            // Проверка тени
            Ray shadowRay;
            shadowRay.origin = hit.point + hit.normal * 0.001;
            shadowRay.direction = lightDir;
            Hit shadowHit = intersectScene(shadowRay);
            float shadow = shadowHit.happened ? 0.2 : 1.0;
            
            finalColor += throughput * hit.color * (ambient + diff * shadow);
            break;
        }
        
        float n1 = AIR_IOR;
        float n2 = refractionIndex;
        bool entering = dot(ray.direction, hit.normal) < 0.0;
        vec3 normal = entering ? hit.normal : -hit.normal;
        
        if (!entering) {
            float temp = n1;
            n1 = n2;
            n2 = temp;
        }
        
        float kr = fresnel(ray.direction, normal, n1, n2);
        
        // Преломление
        if (kr < 0.999) {
            Ray refractRay;
            refractRay.origin = hit.point - normal * 0.001;
            refractRay.direction = snellRefract(ray.direction, normal, n1, n2);
            hit = intersectScene(refractRay);
            
            if (!hit.happened) {
                finalColor += throughput * (1.0 - kr) * vec3(0.1);
                break;
            }
            
            throughput *= (1.0 - kr) * hit.color;
            ray = refractRay;
            continue;
        }
        
        // Отражение
        Ray reflectRay;
        reflectRay.origin = hit.point + normal * 0.001;
        reflectRay.direction = reflect(ray.direction, normal);
        hit = intersectScene(reflectRay);
        
        if (!hit.happened) {
            finalColor += throughput * kr * vec3(0.1);
            break;
        }
        
        throughput *= kr * hit.color;
        ray = reflectRay;
        
        // Прекращаем трассировку, если вклад стал слишком маленьким
        if (length(throughput) < 0.01) break;
    }
    
    return finalColor;
}

void main() {
    vec2 uv = TexCoord * 2.0 - 1.0;
    
    vec3 right = normalize(cross(cameraFront, vec3(0.0, 1.0, 0.0)));
    vec3 up = normalize(cross(right, cameraFront));
    
    Ray ray;
    ray.origin = cameraPos;
    ray.direction = normalize(cameraFront + uv.x * right + uv.y * up);
    
    Hit hit = intersectScene(ray);
    vec3 color = shade(hit, ray);
    
    // Гамма-коррекция
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
}